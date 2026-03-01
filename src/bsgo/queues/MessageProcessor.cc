
#include "MessageProcessor.hh"

namespace bsgo {

MessageProcessor::MessageProcessor(const std::string &onBehalfOfName,
                                   std::deque<bsgalone::core::IMessagePtr> &messages,
                                   std::mutex &locker,
                                   EventHandler handler)
  : bsgalone::core::AbstractEventProcessor<bsgalone::core::IMessage,
                                           bsgalone::core::MessageType>(onBehalfOfName,
                                                                        messages,
                                                                        locker,
                                                                        std::move(handler))
{}

auto MessageProcessor::unimportantEventTypes() const
  -> std::unordered_set<bsgalone::core::MessageType>
{
  return std::unordered_set<bsgalone::core::MessageType>{
    bsgalone::core::MessageType::COMPONENT_SYNC,
    bsgalone::core::MessageType::AI_BEHAVIOR_SYNC,
    bsgalone::core::MessageType::VELOCITY,
  };
}

} // namespace bsgo
