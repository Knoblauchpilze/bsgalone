
#pragma once

#include "IEvent.hh"

namespace net {

class ConnectionLostEvent : public IEvent
{
  public:
  ConnectionLostEvent();
  ~ConnectionLostEvent() override = default;

  auto clone() const -> IEventPtr override;
};

} // namespace net
