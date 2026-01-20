
#pragma once

#include "Context.hh"
#include "CoreObject.hh"
#include "IMessage.hh"
#include <memory>

namespace pge {

class ClientConnection : public core::CoreObject
{
  public:
  ClientConnection(const int port, net::Context &networkContext);
  ~ClientConnection() override = default;

  void setDataHandler(const net::DataReceivedHandler &handler);

  void sendMessage(const bsgo::IMessage &message);

  private:
  net::ConnectionShPtr m_connection{};
};

using ClientConnectionPtr = std::unique_ptr<ClientConnection>;

} // namespace pge
