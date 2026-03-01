
#include "MessageProcessor.hh"

namespace bsgo {

const auto UNIMPORTANT_MESSAGE_TYPES = std::unordered_set<bsgalone::core::MessageType>{
  bsgalone::core::MessageType::COMPONENT_SYNC,
  bsgalone::core::MessageType::AI_BEHAVIOR_SYNC,
  bsgalone::core::MessageType::VELOCITY,
};

auto createMessageProcessor(const std::string &onBehalfOfName,
                            std::deque<bsgalone::core::IMessagePtr> &messages,
                            std::mutex &locker,
                            MessageProcessor::EventHandler handler) -> MessageProcessor
{
  return MessageProcessor(onBehalfOfName,
                          messages,
                          locker,
                          std::move(handler),
                          UNIMPORTANT_MESSAGE_TYPES);
}

} // namespace bsgo
