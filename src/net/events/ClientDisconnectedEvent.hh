
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"

namespace net {

class ClientDisconnectedEvent : public IEvent
{
  public:
  ClientDisconnectedEvent(const ClientId clientId);
  ~ClientDisconnectedEvent() override = default;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
