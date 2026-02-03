
#pragma once

#include "IEvent.hh"

namespace net {

class ConnectionEstablishedEvent : public IEvent
{
  public:
  ConnectionEstablishedEvent();
  ~ConnectionEstablishedEvent() override = default;

  auto clone() const -> IEventPtr override;
};

} // namespace net
