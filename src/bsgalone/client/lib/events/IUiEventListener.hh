
#pragma once

#include "IEvent.hh"
#include "IEventListener.hh"
#include "UiEventType.hh"
#include <memory>

namespace bsgalone::client {

using IUiEventListener    = messaging::IEventListener<UiEventType, IEvent>;
using IUiEventListenerPtr = std::unique_ptr<IUiEventListener>;

} // namespace bsgalone::client
