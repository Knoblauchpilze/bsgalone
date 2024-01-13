
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

} // namespace net
