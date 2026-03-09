
#pragma once

#include "INetworkEvent.hh"

namespace net {

class ConnectionEstablishedEvent : public INetworkEvent
{
  public:
  ConnectionEstablishedEvent();
  ~ConnectionEstablishedEvent() override = default;

  auto clone() const -> INetworkEventPtr override;
};

} // namespace net
