
#pragma once

#include "AbstractEvent.hh"
#include "MessageType.hh"
#include <memory>

namespace bsgalone::core {

using IMessage    = messaging::AbstractEvent<MessageType>;
using IMessagePtr = std::unique_ptr<IMessage>;

} // namespace bsgalone::core
