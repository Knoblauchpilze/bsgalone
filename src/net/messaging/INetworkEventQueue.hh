
#pragma once

#include "IEventQueue.hh"
#include "INetworkEvent.hh"
#include "NetworkEventType.hh"
#include <memory>

namespace net {

using INetworkEventQueue      = messaging::IEventQueue<NetworkEventType, INetworkEvent>;
using INetworkEventQueuePtr   = std::unique_ptr<INetworkEventQueue>;
using INetworkEventQueueWPtr  = std::weak_ptr<INetworkEventQueue>;
using INetworkEventQueueShPtr = std::shared_ptr<INetworkEventQueue>;

} // namespace net
