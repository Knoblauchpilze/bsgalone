
#pragma once

#include "IEventListener.hh"
#include "IUiEvent.hh"
#include "UiEventType.hh"
#include <memory>

namespace bsgalone::client {

using IUiEventListener    = messaging::IEventListener<UiEventType, IUiEvent>;
using IUiEventListenerPtr = std::unique_ptr<IUiEventListener>;

} // namespace bsgalone::client
