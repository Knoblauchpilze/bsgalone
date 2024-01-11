
#include "Connection.hh"

namespace net {

Connection::Connection(asio::io_context &context)
  : utils::CoreObject("connection")
  , m_socket(context)
{
  setService("net");
}

bool Connection::isConnected() const
{
  return m_socket.is_open();
}

} // namespace net
