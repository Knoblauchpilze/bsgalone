
#pragma once

#include "IEventQueue.hh"
#include "IUiCommand.hh"
#include "UiCommandType.hh"
#include <memory>

namespace bsgalone::client {

using IUiCommandQueue      = messaging::IEventQueue<UiCommandType, IUiCommand>;
using IUiCommandQueuePtr   = std::unique_ptr<IUiCommandQueue>;
using IUiCommandQueueShPtr = std::shared_ptr<IUiCommandQueue>;

} // namespace bsgalone::client
