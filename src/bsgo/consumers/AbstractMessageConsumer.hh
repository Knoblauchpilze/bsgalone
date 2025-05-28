
#pragma once

#include "AbstractMessageListener.hh"
#include "CoreObject.hh"

namespace bsgo {

class AbstractMessageConsumer : public AbstractMessageListener, public core::CoreObject
{
  public:
  AbstractMessageConsumer(const std::string &name,
                          const std::unordered_set<MessageType> &relevantMessageTypes);
  ~AbstractMessageConsumer() override = default;
};

} // namespace bsgo
