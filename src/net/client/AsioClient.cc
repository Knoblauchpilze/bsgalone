
#include "AsioClient.hh"
#include "AsioUtils.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "SynchronizedEventBus.hh"

#include <iostream>

namespace net::details {
namespace {
class ClientListenerProxy : public IEventListener
{
  public:
  ClientListenerProxy(std::weak_ptr<IEventListener> listener)
    : m_listener(std::move(listener))
  {}

  ~ClientListenerProxy() override
  {
    std::cout << "[client proxy] deleting proxy\n";
  }

  bool isEventRelevant(const EventType &type) const
  {
    auto listener = m_listener.lock();
    if (listener)
    {
      std::cout << "[proxy] acquiring lock, event " << str(type) << " is forwarded\n";
      return listener->isEventRelevant(type);
    }

    std::cout << "[proxy] could not acquire lock, event " << str(type) << " is not relevant\n";
    return false;
  }

  void onEventReceived(const IEvent &event)
  {
    std::cout << "[proxy] receieved " << str(event.type()) << "\n";
    auto listener = m_listener.lock();
    if (listener)
    {
      std::cout << "[proxy] forwarding " << str(event.type()) << "\n";
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
namespace {
auto shutdownSocket(asio::ip::tcp::socket &socket) -> std::error_code
{
  std::error_code code;
  socket.shutdown(asio::ip::tcp::socket::shutdown_both, code);
  return code;
}
} // namespace

AsioClient::~AsioClient()
{
  std::cout << "[client] starting deletion of client\n";
  if (m_socket)
  {
    const auto code = shutdownSocket(*m_socket);
    if (code)
    {
      warn("Got error while closing socket: " + code.message() + " (" + std::to_string(code.value())
           + ")");
    }
  }
  // TODO: This is not enough, the socket still exist in the context. Maybe what would be
  // needed would be a condition variable to wait for the last event received (i.e. the
  // data read failure) to finish the destructor so that we have a chance to process it.
  // Not sure what happens when the context is stopped before though. Maybe asio handles
  // it gracefully.
  // Something to double check when the implementation is replaced probably.
  // We could also have a promise and set it to true if EOF (code 2) is detected or if
  // the shutting down state is there (to be added to the connection status)
  m_reader.reset();
  m_writer.reset();
  m_socket.reset();
  std::cout << "[client] deleted all pointers\n";
}

void AsioClient::connect(AsioContext &context, const std::string &url, const int port)
{
  auto expected = ConnectionStatus::DISCONNECTED;
  if (!m_status.compare_exchange_strong(expected, ConnectionStatus::CONNECTING))
  {
    throw std::runtime_error("Got unexpected state for asio client (" + str(expected) + ")");
  }

  // https://stackoverflow.com/questions/50557861/weak-from-this-within-constructor
  // TODO: Should be registered only once
  // Maybe we could have a `reconnecting` and a `connection_failed` as statuses
  m_internalBus->addListener(std::make_unique<ClientListenerProxy>(this->weak_from_this()));

  std::cout << "[client] trying to connect\n";

  m_socket = std::make_shared<asio::ip::tcp::socket>(context.get());

  auto endpoints = context.resolve(url, port);
  asio::async_connect(*m_socket,
                      endpoints,
                      std::bind(&AsioClient::onConnectionEstablished,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));

  std::cout << "[client] registered async connect\n";
}

bool AsioClient::isEventRelevant(const EventType &type) const
{
  // Either the events are forwarded as is or they are handled locally
  // so all of them are interesting.
  if (type == EventType::DATA_READ_FAILURE || type == EventType::DATA_WRITE_FAILURE)
  {
    return true;
  }
  return true;
}

void AsioClient::onEventReceived(const IEvent &event)
{
  std::cout << "[client] handling " << str(event.type()) << "\n";
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
  const auto status = m_status.load();
  if (status != ConnectionStatus::CONNECTED)
  {
    error("Cannot send message, invalid connection state", "Current state is " + str(status));
  }

  const auto messageId = m_nextMessageId.fetch_add(1);
  m_writer->send(messageId, std::move(bytes));

  return messageId;
}

void AsioClient::onConnectionEstablished(const std::error_code &code,
                                         const asio::ip::tcp::endpoint &endpoint)
{
  std::cout << "[client] got connection to " << net::str(endpoint) << "\n";
  if (code)
  {
    /// TODO: Maybe this is a bigger failure but for now we allow failing to
    /// contact the server for development purposes.
    warn("Error detected when connecting to " + net::str(endpoint) + " ("
           + std::to_string(code.value()) + ")",
         code.message());
    return;
  }

  std::cout << "[client] priming with reading task\n";
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
    return;
  }
  if (contentLength != sizeof(ClientId))
  {
    warn("Received incorrect amount of bytes for client identifier, expected "
           + std::to_string(sizeof(ClientId)),
         "Received " + std::to_string(contentLength) + " byte(s)");
    return;
  }

  std::cout << "[client] setting up connection\n";
  setupConnection();
  m_status.store(ConnectionStatus::CONNECTED);

  info("Successfully connected as client " + net::str(m_clientId));

  m_eventBus->pushEvent(std::make_unique<ClientConnectedEvent>(m_clientId));
}

void AsioClient::setupConnection()
{
  std::cout << "[client] setup reader and writer\n";
  m_reader = std::make_shared<ReadingSocket>(m_clientId, m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(m_clientId, m_socket, m_internalBus);
  m_reader->connect();
  std::cout << "[client] both sockets set\n";
}

void AsioClient::handleConnectionFailure(const IEvent & /*event*/)
{
  debug("Detected network failure");
  m_eventBus->pushEvent(std::make_unique<ClientDisconnectedEvent>(m_clientId));
  m_status.store(ConnectionStatus::DISCONNECTED);
}

} // namespace net::details
