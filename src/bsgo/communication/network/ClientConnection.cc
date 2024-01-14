
#include "ClientConnection.hh"

namespace bsgo {

ClientConnection::ClientConnection(net::Context &networkContext)
  : utils::CoreObject("connection")
  , m_connection(networkContext.createConnection())
{
  setService("network");
}

void ClientConnection::sendMessage(const IMessage &message)
{
  warn("should message with type " + str(message.type()));
  /// TODO: Handle sending messages.
}

} // namespace bsgo
