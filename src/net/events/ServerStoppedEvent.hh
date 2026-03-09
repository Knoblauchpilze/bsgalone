
#pragma once

#include "INetworkEvent.hh"

namespace net {

class ServerStoppedEvent : public INetworkEvent
{
  public:
  ServerStoppedEvent();
  ~ServerStoppedEvent() override = default;

  auto clone() const -> INetworkEventPtr override;
};

} // namespace net
