
#pragma once

#include "EventType.hh"
#include "IEventListener.hh"
#include "INetworkEvent.hh"
#include <memory>

namespace net {

using INetworkEventListener    = messaging::IEventListener<EventType, INetworkEvent>;
using INetworkEventListenerPtr = std::unique_ptr<INetworkEventListener>;

} // namespace net
