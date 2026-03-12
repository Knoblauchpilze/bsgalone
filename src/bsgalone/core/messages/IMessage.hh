
#pragma once

#include "AbstractEvent.hh"
#include "CoreObject.hh"
#include "MessageType.hh"
#include <memory>

namespace bsgalone::core {

using IMessage    = messaging::AbstractEvent<MessageType>;
using IMessagePtr = std::unique_ptr<IMessage>;

auto operator<<(std::ostream &out, const IMessage &message) -> std::ostream &;

} // namespace bsgalone::core
