
#pragma once

#include "AbstractEvent.hh"
#include "EventType.hh"
#include <memory>

namespace net {

using INetworkEvent    = messaging::AbstractEvent<EventType>;
using INetworkEventPtr = std::unique_ptr<INetworkEvent>;

} // namespace net
