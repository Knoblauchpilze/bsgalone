
#pragma once

#include "IUiEventQueue.hh"
#include <memory>

namespace bsgalone::client {

auto createAsyncUiCommandQueue(IUiEventQueuePtr queue) -> IUiEventQueuePtr;

} // namespace bsgalone::client
