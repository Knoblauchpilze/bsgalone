
#pragma once

#include "IEvent.hh"

namespace net {

class ServerStoppedEvent : public IEvent
{
  public:
  ServerStoppedEvent();
  ~ServerStoppedEvent() override = default;

  auto clone() const -> IEventPtr override;
};

} // namespace net
