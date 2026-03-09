
#pragma once

#include "INetworkEvent.hh"

namespace net {

class ConnectionLostEvent : public INetworkEvent
{
  public:
  ConnectionLostEvent();
  ~ConnectionLostEvent() override = default;

  auto clone() const -> INetworkEventPtr override;
};

} // namespace net
