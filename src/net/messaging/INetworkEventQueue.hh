
#pragma once

#include "EventType.hh"
#include "IEventQueue.hh"
#include "INetworkEvent.hh"
#include <memory>

namespace net {

using INetworkEventQueue      = messaging::IEventQueue<EventType, INetworkEvent>;
using INetworkEventQueuePtr   = std::unique_ptr<INetworkEventQueue>;
using INetworkEventQueueShPtr = std::shared_ptr<INetworkEventQueue>;

} // namespace net
