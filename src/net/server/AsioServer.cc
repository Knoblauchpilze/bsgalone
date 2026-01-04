
#include "AsioServer.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "SynchronizedEventBus.hh"

#include <iostream>

namespace net::details {
namespace {
class ServerListenerProxy : public IEventListener
{
  public:
  ServerListenerProxy(IEventListener *listener)
    : m_listener(listener)
  {}

  ~ServerListenerProxy() override
  {
    std::cout << "[proxy] deleting ServerListenerProxy\n";
  }

  bool isEventRelevant(const EventType & /*type*/) const
  {
    return true;
  }

  void onEventReceived(const IEvent &event)
  {
    m_listener->onEventReceived(event);
  }

  private:
  IEventListener *m_listener{};
};
} // namespace

AsioServer::AsioServer(AsioContext &context, const int port, IEventBusShPtr eventBus)
  : core::CoreObject("server")
  , m_acceptor(context.get(), asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  , m_eventBus(std::move(eventBus))
  , m_internalBus(std::make_shared<AsyncEventBus>(std::make_unique<SynchronizedEventBus>()))
{
  setService("net");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }

  m_internalBus->addListener(std::make_unique<ServerListenerProxy>(this));
}

AsioServer::~AsioServer()
{
  std::cout << "[server] deleting AsioServer\n";
}

void AsioServer::start()
{
  auto expected = false;
  if (!m_registered.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for asio server, did you already call the start method?");
  }

  registerToAsio();
}

bool AsioServer::isEventRelevant(const EventType &type) const
{
  return type == EventType::DATA_READ_FAILURE || type == EventType::DATA_WRITE_FAILURE;
}

void AsioServer::onEventReceived(const IEvent &event)
{
  std::cout << "[server] should handle " << str(event.type()) << "\n";
}

void AsioServer::registerToAsio()
{
  // https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ReadHandler.html
  m_acceptor.async_accept(std::bind(&AsioServer::onConnectionRequest,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

void AsioServer::onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
{
  registerToAsio();

  if (code)
  {
    warn("Error detected when processing incoming connection", code.message());
    return;
  }

  const auto clientId = registerConnection(std::move(socket));
  publishClientConnectedEvent(clientId);
}

auto AsioServer::registerConnection(asio::ip::tcp::socket rawSocket) -> ClientId
{
  auto socket = std::make_shared<asio::ip::tcp::socket>(std::move(rawSocket));

  const auto clientId = m_nextClientId.fetch_add(1);

  auto reader = std::make_shared<ReadingSocket>(clientId, socket, m_internalBus);
  auto writer = std::make_shared<WritingSocket>(clientId, socket, m_internalBus);

  reader->connect();

  const std::lock_guard guard(m_connectionsLocker);
  m_readers.emplace(clientId, std::move(reader));
  m_writers.emplace(clientId, std::move(writer));

  return clientId;
}

void AsioServer::publishClientConnectedEvent(const ClientId clientId)
{
  auto event = std::make_unique<ClientConnectedEvent>(clientId);
  m_eventBus->pushEvent(std::move(event));
}

} // namespace net::details
