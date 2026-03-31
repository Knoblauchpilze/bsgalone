
#pragma once

#include "AbstractEvent.hh"
#include "UiEventType.hh"
#include <memory>

namespace bsgalone::client {

using IUiEvent    = messaging::AbstractEvent<UiEventType>;
using IUiEventPtr = std::unique_ptr<IUiEvent>;

} // namespace bsgalone::client
