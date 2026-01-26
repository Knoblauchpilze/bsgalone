
#include "AsioClient.hh"
#include "AsioUtils.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "SynchronizedEventBus.hh"

namespace net::details {
namespace {
class ClientListenerProxy : public IEventListener
{
  public:
  ClientListenerProxy(std::weak_ptr<IEventListener> listener)
    : m_listener(std::move(listener))
  {}

  ~ClientListenerProxy() override = default;

  bool isEventRelevant(const EventType &type) const
  {
    auto listener = m_listener.lock();
    if (listener)
    {
      return listener->isEventRelevant(type);
    }

    return false;
  }

  void onEventReceived(const IEvent &event)
  {
    auto listener = m_listener.lock();
    if (listener)
    {
      listener->onEventReceived(event);
    }
  }

  private:
  std::weak_ptr<IEventListener> m_listener{};
};
} // namespace

AsioClient::AsioClient(IEventBusShPtr eventBus)
  : core::CoreObject("client")
  , m_eventBus(std::move(eventBus))
  , m_internalBus(std::make_shared<AsyncEventBus>(std::make_unique<SynchronizedEventBus>()))
{
  setService("net");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void AsioClient::connect(AsioContext &context, const std::string &url, const int port)
{
  std::unique_lock guard(m_locker);
  if (m_status != ConnectionStatus::DISCONNECTED)
  {
    error("Got unexpected state for asio client (" + str(m_status) + ")");
  }
  m_status = ConnectionStatus::CONNECTING;

  // This needs to happen after the constructor otherwise the weak_ptr from this
  // is not yet fully initialized. See:
  // https://stackoverflow.com/questions/50557861/weak-from-this-within-constructor
  auto expectedRegistration = false;
  if (m_registeredToInternalBus.compare_exchange_strong(expectedRegistration, true))
  {
    m_internalBus->addListener(std::make_unique<ClientListenerProxy>(this->weak_from_this()));
  }

  m_socket = std::make_shared<asio::ip::tcp::socket>(context.get());

  auto endpoints = context.resolve(url, port);
  asio::async_connect(*m_socket,
                      endpoints,
                      std::bind(&AsioClient::onConnectionEstablished,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));
}

namespace {
auto shutdownSocket(asio::ip::tcp::socket &socket) -> std::error_code
{
  std::error_code code;
  socket.shutdown(asio::ip::tcp::socket::shutdown_both, code);
  return code;
}
} // namespace

void AsioClient::disconnect()
{
  std::unique_lock guard(m_locker);
  if (m_socket)
  {
    const auto code = shutdownSocket(*m_socket);
    if (code)
    {
      warn("Got error while closing socket: " + code.message() + " (" + std::to_string(code.value())
           + ")");
    }
  }

  m_reader.reset();
  m_writer.reset();
  m_socket.reset();

  // This makes sure that we correctly wait until the socket is disconnected before
  // returning from this method.
  m_notifier.wait(guard, [this] { return m_status == ConnectionStatus::DISCONNECTED; });
}

bool AsioClient::isEventRelevant(const EventType & /*type*/) const
{
  // Either the events are forwarded as is or they are handled locally
  // so all of them are interesting.
  return true;
}

void AsioClient::onEventReceived(const IEvent &event)
{
  switch (event.type())
  {
    case EventType::DATA_READ_FAILURE:
    case EventType::DATA_WRITE_FAILURE:
      handleConnectionFailure(event);
      break;
    default:
      m_eventBus->pushEvent(event.clone());
      break;
  }
}

auto AsioClient::trySend(std::vector<char> bytes) -> std::optional<MessageId>
{
  if (bytes.empty())
  {
    warn("Discarding empty message");
    return {};
  }

  std::unique_lock guard(m_locker);
  if (m_status != ConnectionStatus::CONNECTED)
  {
    error("Cannot send message, invalid connection state", "Current state is " + str(m_status));
  }

  const auto messageId = m_nextMessageId.fetch_add(1);
  m_writer->send(messageId, std::move(bytes));

  return messageId;
}

void AsioClient::onConnectionEstablished(const std::error_code &code,
                                         const asio::ip::tcp::endpoint &endpoint)
{
  if (code)
  {
    /// TODO: Maybe this is a bigger failure but for now we allow failing to
    /// contact the server for development purposes.
    warn("Error detected when connecting to " + str(endpoint) + " (" + std::to_string(code.value())
           + ")",
         code.message());

    setConnectionStatusAndNotify(ConnectionStatus::DISCONNECTED);
    return;
  }

  std::unique_lock guard(m_locker);

  // This test is necessary because it can happen that the `disconnect` method is called while the
  // client is establishing the connection to the server. In this case, the client will receive an
  // error in the `onConnectionEstablished` callback and before registering the reading task it is
  // needed to verify that the socket is still a valid object as it's being destroyed in `disconnect`.
  if (!m_socket)
  {
    m_status = ConnectionStatus::DISCONNECTED;
    m_notifier.notify_one();
    return;
  }

  asio::async_read(*m_socket,
                   asio::buffer(&m_clientId, sizeof(ClientId)),
                   std::bind(&AsioClient::onDataReceived,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2));
}

void AsioClient::onDataReceived(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Failed to receive client identifier (" + std::to_string(code.value()) + ")",
         code.message());
    setConnectionStatusAndNotify(ConnectionStatus::DISCONNECTED);
    return;
  }
  if (contentLength != sizeof(ClientId))
  {
    warn("Received incorrect amount of bytes for client identifier, expected "
           + std::to_string(sizeof(ClientId)),
         "Received " + std::to_string(contentLength) + " byte(s)");
    setConnectionStatusAndNotify(ConnectionStatus::DISCONNECTED);
    return;
  }

  setupConnection();

  info("Successfully connected as client " + net::str(m_clientId));

  m_eventBus->pushEvent(std::make_unique<ClientConnectedEvent>(m_clientId));
}

void AsioClient::setupConnection()
{
  std::unique_lock guard(m_locker);
  m_reader = std::make_shared<ReadingSocket>(m_clientId, m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(m_clientId, m_socket, m_internalBus);
  m_reader->connect();
  m_status = ConnectionStatus::CONNECTED;
}

void AsioClient::handleConnectionFailure(const IEvent & /*event*/)
{
  debug("Detected network failure");
  m_eventBus->pushEvent(std::make_unique<ClientDisconnectedEvent>(m_clientId));
  setConnectionStatusAndNotify(ConnectionStatus::DISCONNECTED);
}

void AsioClient::setConnectionStatusAndNotify(const ConnectionStatus status)
{
  std::unique_lock guard(m_locker);
  m_status = status;
  m_notifier.notify_one();
}

} // namespace net::details
