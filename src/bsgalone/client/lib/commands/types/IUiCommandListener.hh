
#pragma once

#include "IEventListener.hh"
#include "IUiCommand.hh"
#include "UiCommandType.hh"
#include <memory>

namespace bsgalone::client {

using IUiCommandListener    = messaging::IEventListener<UiCommandType, IUiCommand>;
using IUiCommandListenerPtr = std::unique_ptr<IUiCommandListener>;

} // namespace bsgalone::client
