
#include "Connection.hh"
#include "AsioUtils.hh"

namespace net {

Connection::Connection(asio::io_context &context)
  : Connection(asio::ip::tcp::socket(context), ConnectionType::CLIENT)
{}

Connection::Connection(asio::ip::tcp::socket &&socket)
  : Connection(std::move(socket), ConnectionType::SERVER)
{}

Connection::Connection(asio::ip::tcp::socket &&socket, const ConnectionType type)
  : utils::CoreObject("connection")
  , m_type(type)
  , m_socket(std::move(socket))
  , m_incomingDataTempBuffer(INCOMING_DATA_BUFFER_SIZE, 0)
{
  setService("net");
}

auto Connection::str() const -> std::string
{
  const auto client    = net::str(m_socket.remote_endpoint());
  const auto connected = isConnected() ? "ON" : "OFF";
  return client + "-" + net::str(m_type) + "-" + connected;
}

auto Connection::type() const -> ConnectionType
{
  return m_type;
}

bool Connection::isConnected() const
{
  return m_socket.is_open();
}

void Connection::activate()
{
  registerToAsio();
}

void Connection::setDataHandler(const DataReceivedHandler &dataHandler)
{
  m_dataHandler = dataHandler;
}

void Connection::registerToAsio()
{
  switch (m_type)
  {
    case ConnectionType::SERVER:
      registerServerConnectionToAsio();
      break;
    case ConnectionType::CLIENT:
    default:
      throw std::invalid_argument("Unsupported connection type " + net::str(m_type));
  }
}

void Connection::registerServerConnectionToAsio()
{
  asio::async_read(m_socket,
                   asio::buffer(m_incomingDataTempBuffer.data(), m_incomingDataTempBuffer.size()),
                   std::bind(&Connection::onDataReceived,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2));
}

void Connection::onDataReceived(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when receiving data for connection", code.message());
    m_socket.close();
    return;
  }

  registerToAsio();

  debug("Received " + std::to_string(contentLength) + " byte(s) on " + str());
  if (!m_dataHandler)
  {
    warn("Discarding " + std::to_string(contentLength) + " byte(s) as there's no data handler");
    return;
  }

  std::move(std::begin(m_incomingDataTempBuffer),
            std::begin(m_incomingDataTempBuffer) + contentLength,
            std::back_inserter(m_partialMessageData));

  const auto processed = (*m_dataHandler)(m_partialMessageData);
  m_partialMessageData.erase(m_partialMessageData.begin(), m_partialMessageData.begin() + processed);
  debug("Processed " + std::to_string(processed) + " byte(s), "
        + std::to_string(m_partialMessageData.size()) + " byte(s) remaining");
}

} // namespace net
