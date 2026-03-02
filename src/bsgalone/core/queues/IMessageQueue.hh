
#pragma once

#include "IEventQueue.hh"
#include "IMessage.hh"
#include "IMessageListener.hh"
#include "MessageType.hh"
#include <memory>

namespace bsgalone::core {

using IMessageQueue      = messaging::IEventQueue<MessageType, IMessage>;
using IMessageQueuePtr   = std::unique_ptr<IMessageQueue>;
using IMessageQueueShPtr = std::shared_ptr<IMessageQueue>;

} // namespace bsgalone::core
