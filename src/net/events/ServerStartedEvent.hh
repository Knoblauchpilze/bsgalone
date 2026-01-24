
#pragma once

#include "IEvent.hh"

namespace net {

class ServerStartedEvent : public IEvent
{
  public:
  ServerStartedEvent();
  ~ServerStartedEvent() override = default;

  auto clone() const -> IEventPtr override;
};

} // namespace net
