
#pragma once

#include "AbstractEvent.hh"
#include "NetworkEventType.hh"
#include <memory>

namespace net {

using INetworkEvent    = messaging::AbstractEvent<NetworkEventType>;
using INetworkEventPtr = std::unique_ptr<INetworkEvent>;

} // namespace net
