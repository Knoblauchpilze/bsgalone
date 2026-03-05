
#pragma once

#include "IMessageQueue.hh"
#include <memory>

namespace bsgo {

auto createAsyncMessageQueue(bsgalone::core::IMessageQueuePtr queue)
  -> bsgalone::core::IMessageQueueShPtr;

} // namespace bsgo
