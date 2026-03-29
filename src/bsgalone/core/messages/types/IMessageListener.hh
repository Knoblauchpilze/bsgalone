
#pragma once

#include "IEventListener.hh"
#include "IMessage.hh"
#include <memory>

namespace bsgalone::core {

using IMessageListener    = messaging::IEventListener<MessageType, IMessage>;
using IMessageListenerPtr = std::unique_ptr<IMessageListener>;

} // namespace bsgalone::core
