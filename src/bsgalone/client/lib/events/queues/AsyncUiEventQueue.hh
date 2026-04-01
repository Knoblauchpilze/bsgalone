
#pragma once

#include "IUiEventQueue.hh"
#include <memory>

namespace bsgalone::client {

auto createAsyncUiEventQueue(IUiEventQueuePtr queue) -> IUiEventQueuePtr;

} // namespace bsgalone::client
