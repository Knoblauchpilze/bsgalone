
#include "Connection.hh"
#include "AsioUtils.hh"

namespace net {
std::atomic<ConnectionId> Connection::NEXT_ID{0};
// Maximum amount of bytes that can be read at once.
constexpr auto KILOBYTES                                     = 1'024;
constexpr auto INCOMING_SERVER_DATA_MAX_BUFFER_SIZE_IN_BYTES = 10 * KILOBYTES;
constexpr auto INCOMING_CLIENT_DATA_MAX_BUFFER_SIZE_IN_BYTES = 10 * KILOBYTES;

Connection::Connection(const std::string &url, const int port, asio::io_context &context)
  : Connection(asio::ip::tcp::socket(context), ConnectionType::CLIENT)
{
  asio::ip::tcp::resolver resolver(context);
  m_endpoints = resolver.resolve(url, std::to_string(port));
}

Connection::Connection(asio::ip::tcp::socket &&socket)
  : Connection(std::move(socket), ConnectionType::SERVER)
{}

Connection::Connection(asio::ip::tcp::socket &&socket, const ConnectionType type)
  : utils::CoreObject("connection")
  , m_id(NEXT_ID.fetch_add(1))
  , m_type(type)
  , m_socket(std::move(socket))

{
  setService("net");

  const auto tempBufferSize = ConnectionType::SERVER == m_type
                                ? INCOMING_SERVER_DATA_MAX_BUFFER_SIZE_IN_BYTES
                                : INCOMING_CLIENT_DATA_MAX_BUFFER_SIZE_IN_BYTES;
  m_incomingDataTempBuffer.resize(tempBufferSize, 0);
}

auto Connection::str() const -> std::string
{
  const auto client    = net::str(m_socket.remote_endpoint());
  const auto connected = isConnected() ? "ON" : "OFF";
  return std::to_string(m_id) + "-" + client + "-" + net::str(m_type) + "-" + connected;
}

auto Connection::id() const -> ConnectionId
{
  return m_id;
}

auto Connection::type() const -> ConnectionType
{
  return m_type;
}

bool Connection::isConnected() const
{
  return m_socket.is_open();
}

void Connection::connect()
{
  switch (m_type)
  {
    case ConnectionType::SERVER:
      registerReadingTaskToAsio();
      break;
    case ConnectionType::CLIENT:
      registerConnectingTaskToAsio();
      break;
    default:
      throw std::invalid_argument("Unsupported connection type " + net::str(m_type));
  }
}

void Connection::setDataHandler(const DataReceivedHandler &dataHandler)
{
  m_dataHandler = dataHandler;
}

void Connection::setDisconnectHandler(const DisconnectHandler &disconnectHandler)
{
  m_disconnectHandler = disconnectHandler;
}

void Connection::registerReadingTaskToAsio()
{
  m_socket.async_read_some(asio::buffer(m_incomingDataTempBuffer.data(),
                                        m_incomingDataTempBuffer.size()),
                           std::bind(&Connection::onDataReceived,
                                     shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void Connection::registerConnectingTaskToAsio()
{
  if (!m_endpoints)
  {
    error("Failed to register client connection", "No endpoints provided");
  }

  asio::async_connect(m_socket,
                      *m_endpoints,
                      std::bind(&Connection::onConnectionEstablished,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));
}

void Connection::registerMessageSendingTaskToAsio()
{
  const std::lock_guard guard(m_dataLock);
  if (m_messagesToSend.empty())
  {
    return;
  }
  if (m_sendingData)
  {
    // Already one message being sent, do not register another task.
    return;
  }

  const auto &message = m_messagesToSend.front();
  asio::async_write(m_socket,
                    asio::buffer(message->data.data(), message->data.size()),
                    std::bind(&Connection::onDataSent,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));

  m_sendingData = true;
}

void Connection::registerMessageToSend(MessageToSendPtr &&message)
{
  {
    const std::lock_guard guard(m_dataLock);
    m_messagesToSend.emplace_back(std::move(message));
  }

  registerMessageSendingTaskToAsio();
}

void Connection::onConnectionEstablished(const std::error_code &code,
                                         const asio::ip::tcp::endpoint &endpoint)
{
  if (code)
  {
    /// TODO: Maybe this is a bigger failure but for now we allow failing to
    /// contact the server for development purposes.
    warn("Error detected when connecting to " + net::str(endpoint), code.message());
    m_socket.close();
    return;
  }

  info("Successfully connected to " + net::str(endpoint));

  registerReadingTaskToAsio();
}

void Connection::onDataReceived(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when receiving data for connection", code.message());
    if (m_disconnectHandler)
    {
      (*m_disconnectHandler)(m_id);
    }

    m_socket.close();
    return;
  }

  verbose("Received " + std::to_string(contentLength) + " byte(s) on " + str());

  if (m_dataHandler)
  {
    std::move(std::begin(m_incomingDataTempBuffer),
              std::begin(m_incomingDataTempBuffer) + contentLength,
              std::back_inserter(m_partialMessageData));

    const auto processed = (*m_dataHandler)(m_id, m_partialMessageData);
    m_partialMessageData.erase(m_partialMessageData.begin(),
                               m_partialMessageData.begin() + processed);
    debug("Processed " + std::to_string(processed) + " byte(s), "
          + std::to_string(m_partialMessageData.size()) + " byte(s) remaining");
  }

  if (!m_dataHandler)
  {
    warn("Discarding " + std::to_string(contentLength) + " byte(s) as there's no data handler");
  }

  registerReadingTaskToAsio();
}

void Connection::onDataSent(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when sending data on connection", code.message());
    if (m_disconnectHandler)
    {
      (*m_disconnectHandler)(m_id);
    }

    m_socket.close();
    return;
  }

  verbose("Sent " + std::to_string(contentLength) + " byte(s) on " + str());

  {
    const std::lock_guard guard(m_dataLock);
    m_messagesToSend.pop_front();
    m_sendingData = false;
  }

  registerMessageSendingTaskToAsio();
}

} // namespace net
