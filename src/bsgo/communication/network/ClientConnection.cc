
#include "ClientConnection.hh"

namespace bsgo {

ClientConnection::ClientConnection(net::Context &networkContext)
  : utils::CoreObject("connection")
  , m_connection(networkContext.createConnection())
{
  setService("network");
}

} // namespace bsgo
