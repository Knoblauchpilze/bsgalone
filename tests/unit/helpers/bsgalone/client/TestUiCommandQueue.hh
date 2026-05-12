
#pragma once

#include "IUiCommand.hh"
#include "TestAbstractEventQueue.hh"
#include "UiCommandType.hh"

namespace test {

using TestUiCommandQueue
  = TestAbstractEventQueue<bsgalone::client::UiCommandType, bsgalone::client::IUiCommand>;

} // namespace test
