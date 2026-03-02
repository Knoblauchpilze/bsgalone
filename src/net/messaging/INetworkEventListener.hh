
#pragma once

#include "EventType.hh"
#include "IEvent.hh"
#include "IEventListener.hh"
#include <memory>

namespace net {

using INetworkEventListener    = messaging::IEventListener<EventType, IEvent>;
using INetworkEventListenerPtr = std::unique_ptr<INetworkEventListener>;

} // namespace net