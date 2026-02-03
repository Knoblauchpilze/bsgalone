
#include "AsioServer.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReadFailureEvent.hh"
#include "DataWriteFailureEvent.hh"
#include "SynchronizedEventBus.hh"

namespace net::details {
namespace {
class ServerListenerProxy : public IEventListener
{
  public:
  ServerListenerProxy(IEventListener *listener)
    : m_listener(listener)
  {}

  ~ServerListenerProxy() override = default;

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

void AsioServer::start()
{
  if (!m_state.markAsRegistered())
  {
    throw std::runtime_error(
      "Got unexpected state for asio server, did you already call the start method?");
  }

  registerToAsio();
}

void AsioServer::shutdown()
{
  if (!m_state.markAsTerminating())
  {
    throw std::runtime_error(
      "Got unexpected state for asio server, did you forget to call the start method?");
  }

  closeSockets();

  std::unique_lock guard(m_connectionsLocker);
  if (!m_sockets.empty())
  {
    m_notifier.wait(guard, [this] { return m_sockets.empty(); });
  }
}

bool AsioServer::isEventRelevant(const EventType & /*type*/) const
{
  // All events are relevant: they are either forwarded or handled locally.
  return true;
}

void AsioServer::onEventReceived(const IEvent &event)
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

auto AsioServer::trySend(const ClientId clientId, std::vector<char> bytes)
  -> std::optional<MessageId>
{
  if (!m_state.isRegistered())
  {
    error("Cannot send message to " + str(clientId), "Server is not started");
  }

  if (bytes.empty())
  {
    warn("Discarding empty message to " + str(clientId));
    return {};
  }

  const std::lock_guard guard(m_connectionsLocker);

  const auto maybeWriter = m_writers.find(clientId);
  if (maybeWriter == m_writers.end())
  {
    warn("Discarding message to " + str(clientId) + ", no such client");
    return {};
  }

  auto messageId = m_nextMessageId.fetch_add(1);
  maybeWriter->second->send(messageId, std::move(bytes));

  return messageId;
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

  auto event = std::make_unique<ClientConnectedEvent>(clientId);
  m_eventBus->pushEvent(std::move(event));
}

namespace {
auto tryGetClientId(const IEvent &event) -> std::optional<ClientId>
{
  switch (event.type())
  {
    case EventType::DATA_READ_FAILURE:
      return event.as<DataReadFailureEvent>().tryGetClientId();
    case EventType::DATA_WRITE_FAILURE:
      return event.as<DataWriteFailureEvent>().tryGetClientId();
    default:
      return {};
  }
}
} // namespace

void AsioServer::handleConnectionFailure(const IEvent &event)
{
  const auto maybeClientId = tryGetClientId(event);
  if (!maybeClientId)
  {
    error("Failed to handle connection failure", "Received unsupported event " + str(event.type()));
  }

  {
    const std::lock_guard guard(m_connectionsLocker);
    m_writers.erase(*maybeClientId);
    m_readers.erase(*maybeClientId);
    m_sockets.erase(*maybeClientId);

    m_notifier.notify_one();
  }

  debug("Detected failure for client " + str(*maybeClientId) + ", removing connection");

  auto out = std::make_unique<ClientDisconnectedEvent>(*maybeClientId);
  m_eventBus->pushEvent(std::move(out));
}

auto AsioServer::registerConnection(asio::ip::tcp::socket rawSocket) -> net::ClientId
{
  const auto clientId = m_nextClientId.fetch_add(1);

  auto socket = std::make_shared<asio::ip::tcp::socket>(std::move(rawSocket));
  auto reader = std::make_shared<ReadingSocket>(clientId, socket, m_internalBus);
  auto writer = std::make_shared<WritingSocket>(clientId, socket, m_internalBus);

  reader->connect();

  const std::lock_guard guard(m_connectionsLocker);
  m_sockets.emplace(clientId, std::move(socket));
  m_readers.emplace(clientId, std::move(reader));
  m_writers.emplace(clientId, std::move(writer));

  return clientId;
}

namespace {
auto shutdownSocket(asio::ip::tcp::socket &socket) -> std::error_code
{
  std::error_code code;
  socket.shutdown(asio::ip::tcp::socket::shutdown_both, code);
  return code;
}
} // namespace

void AsioServer::closeSockets()
{
  const std::lock_guard guard(m_connectionsLocker);

  for (const auto &[_, socket] : m_sockets)
  {
    const auto code = shutdownSocket(*socket);
    if (code)
    {
      warn("Got error while closing socket: " + code.message() + " (" + std::to_string(code.value())
           + ")");
    }
  }
}

} // namespace net::details
