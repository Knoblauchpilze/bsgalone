
#include "SynchronizedMessageQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::core {

const auto UNIMPORTANT_MESSAGE_TYPES = std::unordered_set<MessageType>{};

auto createSynchronizedMessageQueue() -> IMessageQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<MessageType, IMessage>>(
    allMessageTypesAsSet(), UNIMPORTANT_MESSAGE_TYPES);
}

} // namespace bsgalone::core
