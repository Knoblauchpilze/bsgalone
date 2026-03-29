
#pragma once

#include "IMessageQueue.hh"
#include <memory>

namespace bsgalone::core {

auto createSynchronizedMessageQueue() -> IMessageQueuePtr;

} // namespace bsgalone::core
