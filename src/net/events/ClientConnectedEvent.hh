
#pragma once

#include "ClientId.hh"
#include "INetworkEvent.hh"

namespace net {

class ClientConnectedEvent : public INetworkEvent
{
  public:
  ClientConnectedEvent(const ClientId clientId);
  ~ClientConnectedEvent() override = default;

  auto clientId() const -> ClientId;

  auto clone() const -> INetworkEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
