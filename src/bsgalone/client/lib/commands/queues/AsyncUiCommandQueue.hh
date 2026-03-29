
#pragma once

#include "IUiCommandQueue.hh"
#include <memory>

namespace bsgalone::client {

auto createAsyncUiCommandQueue(IUiCommandQueuePtr queue) -> IUiCommandQueuePtr;

} // namespace bsgalone::client
