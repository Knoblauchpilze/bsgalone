
#pragma once

#include "ClientId.hh"
#include "INetworkEvent.hh"

namespace net {

class ClientDisconnectedEvent : public INetworkEvent
{
  public:
  ClientDisconnectedEvent(const ClientId clientId);
  ~ClientDisconnectedEvent() override = default;

  auto clientId() const -> ClientId;

  auto clone() const -> INetworkEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
