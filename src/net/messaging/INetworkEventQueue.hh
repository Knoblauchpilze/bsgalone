
#pragma once

#include "EventType.hh"
#include "IEvent.hh"
#include "IEventQueue.hh"
#include "INetworkEventListener.hh"
#include <memory>

namespace net {

using INetworkEventQueue      = messaging::IEventQueue<EventType, IEvent>;
using INetworkEventQueuePtr   = std::unique_ptr<INetworkEventQueue>;
using INetworkEventQueueShPtr = std::shared_ptr<INetworkEventQueue>;

} // namespace net