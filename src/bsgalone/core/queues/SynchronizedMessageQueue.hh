
#pragma once

#include "IMessageQueue.hh"
#include <memory>

namespace bsgo {

auto createSynchronizedMessageQueue() -> bsgalone::core::IMessageQueuePtr;

} // namespace bsgo
