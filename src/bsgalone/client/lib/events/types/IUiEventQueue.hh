
#pragma once

#include "IEventQueue.hh"
#include "IUiEvent.hh"
#include "UiEventType.hh"
#include <memory>

namespace bsgalone::client {

using IUiEventQueue      = messaging::IEventQueue<UiEventType, IUiEvent>;
using IUiEventQueuePtr   = std::unique_ptr<IUiEventQueue>;
using IUiEventQueueShPtr = std::shared_ptr<IUiEventQueue>;

} // namespace bsgalone::client
