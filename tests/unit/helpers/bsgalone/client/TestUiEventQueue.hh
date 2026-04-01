
#pragma once

#include "IUiEvent.hh"
#include "TestAbstractEventQueue.hh"
#include "UiEventType.hh"

namespace test {

using TestUiEventQueue
  = TestAbstractEventQueue<bsgalone::client::UiEventType, bsgalone::client::IUiEvent>;

} // namespace test
