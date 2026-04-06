
#pragma once

#include "GameEventType.hh"
#include "IGameEvent.hh"
#include "TestAbstractEventQueue.hh"

namespace test {

using TestGameEventQueue
  = TestAbstractEventQueue<bsgalone::server::GameEventType, bsgalone::server::IGameEvent>;

} // namespace test
