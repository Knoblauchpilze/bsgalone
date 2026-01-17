
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

AsioServer::~AsioServer()
{
  // This is necessary otherwise the sockets get destroyed when the maps are being
  // cleared which leads to asio triggering the callbacks for removing a socket and
  // as the destruction of the maps in this destructor is not protected by a lock
  // we get race conditions.
  const std::lock_guard guard(m_connectionsLocker);
  closeSockets();

  m_writers.clear();
  m_readers.clear();
  m_sockets.clear();
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
    case EventType::DATA_SENT:
      handleHandshakeSuccessOrForward(event.as<DataSentEvent>());
      break;
    default:
      m_eventBus->pushEvent(event.clone());
      break;
  }
}

auto AsioServer::trySend(const ClientId clientId, std::vector<char> bytes)
  -> std::optional<MessageId>
{
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

  const auto clientId = m_nextClientId.fetch_add(1);
  auto socketData     = createSocketData(clientId, std::move(socket));
  initiateHandshake(clientId, std::move(socketData));
}

auto AsioServer::createSocketData(const ClientId clientId, asio::ip::tcp::socket rawSocket) const
  -> SocketData
{
  auto socket = std::make_shared<asio::ip::tcp::socket>(std::move(rawSocket));

  SocketData data{
    .socket = socket,
    .reader = std::make_shared<ReadingSocket>(clientId, socket, m_internalBus),
    .writer = std::make_shared<WritingSocket>(clientId, socket, m_internalBus),
  };

  return data;
}

void AsioServer::initiateHandshake(const ClientId clientId, SocketData data)
{
  std::vector<char> handshake(sizeof(ClientId), 0);
  const auto clientIdBuf = reinterpret_cast<const char *>(&clientId);
  std::copy(clientIdBuf, clientIdBuf + sizeof(ClientId), handshake.data());

  data.handshakeMessageId = m_nextMessageId.fetch_add(1);
  data.writer->send(*data.handshakeMessageId, handshake);

  const std::lock_guard guard(m_connectionsLocker);
  m_pendingSockets.emplace(clientId, std::move(data));
}

namespace {
auto tryGetClientId(const IEvent &event) -> std::optional<ClientId>
{
  switch (event.type())
  {
    case EventType::DATA_READ_FAILURE:
      return event.as<DataReadFailureEvent>().clientId();
    case EventType::DATA_WRITE_FAILURE:
      return event.as<DataWriteFailureEvent>().clientId();
    default:
      return {};
  }
}

auto shutdownSocket(asio::ip::tcp::socket &socket) -> std::error_code
{
  std::error_code code;
  socket.shutdown(asio::ip::tcp::socket::shutdown_both, code);
  return code;
}
} // namespace

void AsioServer::handleConnectionFailure(const IEvent &event)
{
  const auto maybeClientId = tryGetClientId(event);
  if (!maybeClientId)
  {
    error("Failed to handle connection failure", "Received unsupported event " + str(event.type()));
  }

  if (event.type() == EventType::DATA_WRITE_FAILURE
      && tryHandleHandshakeFailure(event.as<DataWriteFailureEvent>()))
  {
    return;
  }

  {
    const std::lock_guard guard(m_connectionsLocker);
    m_writers.erase(*maybeClientId);
    m_readers.erase(*maybeClientId);

    // Note: the socket is shutdown to cancel any pending operations. However if it is
    // removed from the `m_sockets` array it seems we get sometimes a `heap-use-after-free`
    // error from a random integration test.
    // This means that the `m_sockets` is growing the more clients are connecting to the
    // server. If it becomes an issue the random crash should be addressed.
    const auto maybeSocket = m_sockets.find(*maybeClientId);
    if (maybeSocket != m_sockets.end())
    {
      shutdownSocket(*maybeSocket->second);
    }
  }

  debug("Detected failure for client " + str(*maybeClientId) + ", removing connection");

  auto out = std::make_unique<ClientDisconnectedEvent>(*maybeClientId);
  m_eventBus->pushEvent(std::move(out));
}

bool AsioServer::tryHandleHandshakeFailure(const DataWriteFailureEvent &event)
{
  auto maybeSocketData = takePendingSocketData(event.clientId(), event.messageId());
  if (!maybeSocketData)
  {
    return false;
  }

  warn("Detected failure when performing handshake, closing connection");

  shutdownSocket(*maybeSocketData->socket);

  return true;
}

void AsioServer::handleHandshakeSuccessOrForward(const DataSentEvent &event)
{
  auto maybeSocketData = takePendingSocketData(event.clientId(), event.messageId());

  if (!maybeSocketData)
  {
    // The message does not correspond to a pending handshake, forward it.
    m_eventBus->pushEvent(event.clone());
    return;
  }

  registerConnection(event.clientId(), *maybeSocketData);

  auto out = std::make_unique<ClientConnectedEvent>(event.clientId());
  m_eventBus->pushEvent(std::move(out));
}

auto AsioServer::takePendingSocketData(const ClientId clientId, const MessageId expectedMessageId)
  -> std::optional<SocketData>
{
  const std::lock_guard guard(m_connectionsLocker);
  const auto maybeHandshake = m_pendingSockets.find(clientId);

  if (maybeHandshake == m_pendingSockets.end())
  {
    return {};
  }

  if (maybeHandshake->second.handshakeMessageId.value() != expectedMessageId)
  {
    error("Mismatch between the expected handshake message ("
          + str(maybeHandshake->second.handshakeMessageId.value()) + ") and actual message ("
          + str(expectedMessageId) + ")");
  }

  SocketData toRegister;
  std::swap(toRegister, maybeHandshake->second);
  m_pendingSockets.erase(clientId);

  return toRegister;
}

void AsioServer::registerConnection(const ClientId clientId, SocketData data)
{
  data.reader->connect();

  const std::lock_guard guard(m_connectionsLocker);
  m_sockets.emplace(clientId, data.socket);
  m_readers.emplace(clientId, data.reader);
  m_writers.emplace(clientId, data.writer);
}

void AsioServer::closeSockets()
{
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
