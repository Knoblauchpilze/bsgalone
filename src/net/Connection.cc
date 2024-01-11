
#include "Connection.hh"

namespace net {

Connection::Connection(asio::ip::tcp::socket socket)
  : utils::CoreObject("connection")
  , m_socket(std::move(socket))
{
  setService("net");
}

bool Connection::isConnected() const
{
  return m_socket.is_open();
}

} // namespace net
