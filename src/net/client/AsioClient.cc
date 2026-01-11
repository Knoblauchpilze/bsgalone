
#include "AsioClient.hh"
#include "AsioUtils.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
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
  if (m_connected.load())
  {
    m_socket->cancel();
  }
}

void AsioClient::connect(AsioContext &context, const std::string &url, const int port)
{
  auto expected = false;
  if (!m_connected.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for asio client, did you already call the connect method?");
  }

  std::cout << "[client] trying to connect\n";
  if (!tryConnect(context, url, port))
  {
    warn("Failed to connect to " + url + ":" + std::to_string(port));
    return;
  }

  std::cout << "[client] wait for client id\n";
  const auto maybeClientId = tryWaitForClientIdentifier();
  if (!maybeClientId)
  {
    warn("Failed to retrieve client identifier");
  }

  std::cout << "[client] setting up connection\n";
  setupConnection(*maybeClientId);

  info("Successfully connected to " + url + ":" + std::to_string(port) + " as client "
       + str(*maybeClientId));
}

bool AsioClient::isEventRelevant(const EventType & /*type*/) const
{
  // Either the events are forwarded as is or they are handled locally
  // so all of them are interesting.
  return true;
}

void AsioClient::onEventReceived(const IEvent &event)
{
  warn("should handle event " + str(event.type()));
  m_eventBus->pushEvent(event.clone());
}

auto AsioClient::trySend(std::vector<char> bytes) -> std::optional<MessageId>
{
  if (bytes.empty())
  {
    warn("Discarding empty message");
    return {};
  }

  const auto messageId = m_nextMessageId.fetch_add(1);
  m_writer->send(messageId, std::move(bytes));

  return messageId;
}

bool AsioClient::tryConnect(AsioContext &context, const std::string &url, const int port)
{
  m_socket = std::make_shared<asio::ip::tcp::socket>(context.get());

  auto endpoints = context.resolve(url, port);
  const auto success
    = withSafetyNet([this,
                     &endpoints]() { asio::connect(*m_socket, endpoints.begin(), endpoints.end()); },
                    "AsioClient::tryConnect");

  return success;
}

auto AsioClient::tryWaitForClientIdentifier() const -> std::optional<ClientId>
{
  ClientId clientId{};
  const auto expectedBytes = sizeof(ClientId);

  const auto received = asio::read(*m_socket, asio::buffer(&clientId, expectedBytes));
  if (received != expectedBytes)
  {
    warn("Received only " + std::to_string(received) + " byte(s) for client identifier, expected "
         + std::to_string(expectedBytes));
    return {};
  }

  return clientId;
}

void AsioClient::setupConnection(const ClientId clientId)
{
  m_reader = std::make_shared<ReadingSocket>(clientId, m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(clientId, m_socket, m_internalBus);
  m_reader->connect();
}

} // namespace net::details
