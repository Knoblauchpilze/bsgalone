
#include "SynchronizedMessageQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgo {

const auto UNIMPORTANT_MESSAGE_TYPES = std::unordered_set<bsgalone::core::MessageType>{
  bsgalone::core::MessageType::COMPONENT_SYNC,
  bsgalone::core::MessageType::AI_BEHAVIOR_SYNC,
  bsgalone::core::MessageType::VELOCITY,
};

auto createSynchronizedMessageQueue() -> bsgalone::core::IMessageQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<bsgalone::core::MessageType,
                                                                    bsgalone::core::IMessage>>(
    bsgalone::core::allMessageTypesAsSet(), UNIMPORTANT_MESSAGE_TYPES);
}

} // namespace bsgo
