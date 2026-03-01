
#pragma once

#include "AbstractEventProcessor.hh"
#include "IMessage.hh"

namespace bsgo {

using MessageProcessor
  = messaging::AbstractEventProcessor<bsgalone::core::MessageType, bsgalone::core::IMessage>;

auto createMessageProcessor(const std::string &onBehalfOfName,
                            std::deque<bsgalone::core::IMessagePtr> &messages,
                            std::mutex &locker,
                            MessageProcessor::EventHandler handler) -> MessageProcessor;

} // namespace bsgo
