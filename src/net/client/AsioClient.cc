
#include "AsioClient.hh"
#include "AsioUtils.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "SynchronizedEventBus.hh"

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
  m_socket->cancel();
}

void AsioClient::connect(AsioContext &context, const std::string &url, const int port)
{
  auto expected = false;
  if (!m_connected.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for asio client, did you already call the connect method?");
  }

  m_socket = std::make_shared<asio::ip::tcp::socket>(context.get());
  // TODO: The client ID should be updated
  m_reader = std::make_shared<ReadingSocket>(ClientId{0}, m_socket, m_internalBus);
  m_writer = std::make_shared<WritingSocket>(ClientId{0}, m_socket, m_internalBus);

  auto endpoints = context.resolve(url, port);
  asio::async_connect(*m_socket,
                      endpoints,
                      std::bind(&AsioClient::onConnectionEstablished,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));
}

bool AsioClient::isEventRelevant(const EventType & /*type*/) const
{
  // Either the events are forwarded as is or they are handled locally.
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

void AsioClient::onConnectionEstablished(const std::error_code &code,
                                         const asio::ip::tcp::endpoint &endpoint)
{
  if (code)
  {
    /// TODO: Maybe this is a bigger failure but for now we allow failing to
    /// contact the server for development purposes.
    warn("Error detected when connecting to " + str(endpoint), code.message());
    return;
  }

  info("Successfully connected to " + net::str(endpoint));

  // TODO: It should wait for the data read event and update the client identifier
  m_reader->connect();
}

} // namespace net::details
