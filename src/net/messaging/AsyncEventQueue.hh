
#pragma once

#include "INetworkEventQueue.hh"
#include <memory>

namespace net {

auto createAsyncEventQueue(INetworkEventQueuePtr queue) -> INetworkEventQueueShPtr;

} // namespace net
