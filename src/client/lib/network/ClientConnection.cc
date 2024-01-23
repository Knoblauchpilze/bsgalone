
#include "ClientConnection.hh"

namespace pge {
constexpr auto DEFAULT_SERVER_URL  = "127.0.0.1";
constexpr auto DEFAULT_SERVER_PORT = 60000;

ClientConnection::ClientConnection(net::Context &networkContext)
  : utils::CoreObject("connection")
  , m_connection(networkContext.createConnection(DEFAULT_SERVER_URL, DEFAULT_SERVER_PORT))
{
  setService("network");
  m_connection->connect();
}

void ClientConnection::sendMessage(const bsgo::IMessage &message)
{
  m_connection->send(message);
}

} // namespace pge
