
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
  warn("should processed data received (so far: " + std::to_string(contentLength) + ")");

  /// TODO: Read data from connection.
  // // A complete message header has been read, check if this message
  // // has a body to follow...
  // if (m_msgTemporaryIn.header.size > 0)
  // {
  //   // ...it does, so allocate enough space in the messages' body
  //   // vector, and issue asio with the task to read the body.
  //   m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
  //   ReadBody();
  // }
  // else
  // {
  //   // it doesn't, so add this bodyless message to the connections
  //   // incoming message queue
  //   AddToIncomingMessageQueue();
  // }
}

} // namespace net
