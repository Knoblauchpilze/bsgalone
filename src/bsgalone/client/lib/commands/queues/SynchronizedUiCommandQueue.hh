
#pragma once

#include "IUiCommandQueue.hh"
#include <memory>

namespace bsgalone::client {

auto createSynchronizedUiCommandQueue() -> IUiCommandQueuePtr;

} // namespace bsgalone::client
