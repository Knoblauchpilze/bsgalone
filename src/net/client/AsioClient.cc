
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
  ClientListenerProxy(IEventListener *listener)
    : m_listener(listener)
  {}

  ~ClientListenerProxy() override = default;

  bool isEventRelevant(const EventType &type) const
  {
    return m_listener->isEventRelevant(type);
  }

  void onEventReceived(const IEvent &event)
  {
    m_listener->onEventReceived(event);
  }

  private:
  IEventListener *m_listener{};
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

  m_internalBus->addListener(std::make_unique<ClientListenerProxy>(this));
}

AsioClient::~AsioClient()
{
  if (m_status.load() != ConnectionStatus::DISCONNECTED)
  {
    m_socket->cancel();
  }
}

void AsioClient::connect(AsioContext &context, const std::string &url, const int port)
{
  auto expected = ConnectionStatus::DISCONNECTED;
  if (!m_status.compare_exchange_strong(expected, ConnectionStatus::CONNECTING))
  {
    throw std::runtime_error("Got unexpected state for asio client (" + str(expected) + ")");
  }

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
  m_reader = std::make_shared<ReadingSocket>(m_clientId, m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(m_clientId, m_socket, m_internalBus);
  m_reader->connect();
}

void AsioClient::handleConnectionFailure(const IEvent & /*event*/)
{
  debug("Detected network failure");
  m_eventBus->pushEvent(std::make_unique<ClientDisconnectedEvent>(m_clientId));
  m_status.store(ConnectionStatus::DISCONNECTED);
}

} // namespace net::details
