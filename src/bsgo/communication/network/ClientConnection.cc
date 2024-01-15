
#include "ClientConnection.hh"
#include <sstream>

namespace bsgo {
constexpr auto DEFAULT_SERVER_URL  = "127.0.0.1";
constexpr auto DEFAULT_SERVER_PORT = 60000;

ClientConnection::ClientConnection(net::Context &networkContext)
  : utils::CoreObject("connection")
  , m_connection(networkContext.createConnection(DEFAULT_SERVER_URL, DEFAULT_SERVER_PORT))
{
  setService("network");
  m_connection->activate();
}

void ClientConnection::sendMessage(const IMessage &message)
{
  std::ostringstream out{};
  out << message;

  /// TODO: Handle sending messages.
  warn("should message with type " + str(message.type()) + ": \"" + out.str() + "\"");
}

} // namespace bsgo
