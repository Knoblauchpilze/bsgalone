
#include "AsioClient.hh"
#include "AsioUtils.hh"
#include "AsyncEventQueue.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "SynchronizedEventQueue.hh"

namespace net::details {
namespace {
class ClientListenerProxy : public INetworkEventListener
{
  public:
  ClientListenerProxy(std::weak_ptr<INetworkEventListener> listener)
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
  std::weak_ptr<INetworkEventListener> m_listener{};
};
} // namespace

AsioClient::AsioClient(INetworkEventQueueShPtr eventBus)
  : core::CoreObject("client")
  , m_eventBus(std::move(eventBus))
  , m_internalBus(createAsyncEventQueue(createSynchronizedEventQueue()))
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
  if (m_status == ConnectionStatus::DISCONNECTED)
  {
    return;
  }
  if (m_status == ConnectionStatus::DISCONNECTING)
  {
    error("Got unexpected state to disconnect asio client (" + str(m_status) + ")");
  }
  m_status = ConnectionStatus::DISCONNECTING;

  if (m_socket)
  {
    const auto code = shutdownSocket(*m_socket);
    if (code)
    {
      warn("Got error while closing socket: " + code.message() + " (" + std::to_string(code.value())
           + ")");
    }
  }

  // This makes sure that we correctly wait until the socket is disconnected before
  // returning from this method.
  m_notifier.wait(guard, [this] { return m_status == ConnectionStatus::DISCONNECTED; });

  // Destroy the asio objects after we are sure that the socket is closed. This is
  // required by asio to guarantee that the lifetime of the objects is sufficient
  // for all callbacks to be executed.
  // In this case the socket needs to be at least alive until `onConnectionEstablished`
  // has been called. It is therefore needed to wait until the disconnected state
  // is reached.
  m_reader.reset();
  m_writer.reset();
  m_socket.reset();
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

  // This test is necessary because it can happen that the `disconnect` method is called while the
  // client is establishing the connection to the server. In this case, the client will receive an
  // error in the `onConnectionEstablished` callback and before registering the socket and sending
  // the connected event is needed to verify that it still makes sense to do so.
  if (handlePrematureDisconnection())
  {
    return;
  }

  info("Successfully connected to server");

  setupConnection();

  m_eventBus->pushEvent(std::make_unique<ConnectionEstablishedEvent>());
}

bool AsioClient::handlePrematureDisconnection()
{
  std::unique_lock guard(m_locker);

  if (m_status == ConnectionStatus::DISCONNECTING)
  {
    m_status = ConnectionStatus::DISCONNECTED;
    m_notifier.notify_one();
    return true;
  }

  return false;
}

void AsioClient::setupConnection()
{
  std::unique_lock guard(m_locker);
  m_reader = std::make_shared<ReadingSocket>(m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(m_socket, m_internalBus);
  m_reader->connect();
  m_status = ConnectionStatus::CONNECTED;
}

void AsioClient::handleConnectionFailure(const IEvent & /*event*/)
{
  debug("Detected network failure");
  m_eventBus->pushEvent(std::make_unique<ConnectionLostEvent>());
  setConnectionStatusAndNotify(ConnectionStatus::DISCONNECTED);
}

void AsioClient::setConnectionStatusAndNotify(const ConnectionStatus status)
{
  std::unique_lock guard(m_locker);
  m_status = status;
  m_notifier.notify_one();
}

} // namespace net::details
