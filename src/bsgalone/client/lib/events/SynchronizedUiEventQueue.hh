
#pragma once

#include "IUiEventQueue.hh"
#include <memory>

namespace bsgalone::client {

auto createSynchronizedUiEventQueue() -> IUiEventQueuePtr;

} // namespace bsgalone::client
