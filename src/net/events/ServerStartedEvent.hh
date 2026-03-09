
#pragma once

#include "INetworkEvent.hh"

namespace net {

class ServerStartedEvent : public INetworkEvent
{
  public:
  ServerStartedEvent();
  ~ServerStartedEvent() override = default;

  auto clone() const -> INetworkEventPtr override;
};

} // namespace net
