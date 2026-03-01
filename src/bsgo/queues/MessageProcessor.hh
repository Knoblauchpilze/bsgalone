
#pragma once

#include "AbstractEventProcessor.hh"
#include "IMessage.hh"

namespace bsgo {

class MessageProcessor
  : public messaging::AbstractEventProcessor<bsgalone::core::IMessage, bsgalone::core::MessageType>
{
  public:
  MessageProcessor(const std::string &onBehalfOfName,
                   std::deque<bsgalone::core::IMessagePtr> &messages,
                   std::mutex &locker,
                   EventHandler handler);

  protected:
  auto unimportantEventTypes() const -> std::unordered_set<bsgalone::core::MessageType> override;
};

} // namespace bsgo
