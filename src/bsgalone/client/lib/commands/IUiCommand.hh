
#pragma once

#include "AbstractEvent.hh"
#include "UiCommandType.hh"
#include <memory>

namespace bsgalone::client {

using IUiCommand    = messaging::AbstractEvent<UiCommandType>;
using IUiCommandPtr = std::unique_ptr<IUiCommand>;

} // namespace bsgalone::client
