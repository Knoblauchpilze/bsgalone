
#pragma once

#include "IMessage.hh"
#include "MessageType.hh"
#include "TestAbstractEventQueue.hh"

namespace test {

using TestMessageQueue
  = TestAbstractEventQueue<bsgalone::core::MessageType, bsgalone::core::IMessage>;

} // namespace test
