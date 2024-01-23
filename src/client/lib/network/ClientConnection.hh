
#pragma once

#include "Context.hh"
#include "IMessage.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class ClientConnection : public utils::CoreObject
{
  public:
  ClientConnection(net::Context &networkContext);
  ~ClientConnection() override = default;

  void sendMessage(const bsgo::IMessage &message);

  private:
  net::ConnectionShPtr m_connection{};
};

using ClientConnectionPtr = std::unique_ptr<ClientConnection>;

} // namespace pge
