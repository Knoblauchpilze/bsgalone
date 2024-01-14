
#pragma once

#include "Context.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class ClientConnection : public utils::CoreObject
{
  public:
  ClientConnection(net::Context &networkContext);
  ~ClientConnection() override = default;

  private:
  net::ConnectionPtr m_connection{};
};

} // namespace bsgo
