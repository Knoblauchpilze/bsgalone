
#pragma once

#include "IMessageQueue.hh"
#include <memory>

namespace bsgalone::core {

auto createAsyncMessageQueue(IMessageQueuePtr queue) -> IMessageQueueShPtr;

} // namespace bsgalone::core
