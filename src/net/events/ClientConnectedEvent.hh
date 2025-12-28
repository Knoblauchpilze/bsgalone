
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"

namespace net {

class ClientConnectedEvent : public IEvent
{
  public:
  ClientConnectedEvent(const ClientId clientId);
  ~ClientConnectedEvent() override = default;

  auto clientId() const -> ClientId;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
