
#pragma once

#include "INetworkEventQueue.hh"
#include <memory>

namespace net {

auto createSynchronizedEventQueue() -> INetworkEventQueuePtr;

} // namespace net
