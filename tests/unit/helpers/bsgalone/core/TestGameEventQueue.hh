
#pragma once

#include "GameEventType.hh"
#include "IGameEvent.hh"
#include "TestAbstractEventQueue.hh"

namespace test {

using TestGameEventQueue
  = TestAbstractEventQueue<bsgalone::core::GameEventType, bsgalone::core::IGameEvent>;

} // namespace test
