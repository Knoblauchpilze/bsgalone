
#pragma once

#include "AbstractMessageListener.hh"
#include "CoreObject.hh"

namespace bsgalone::core {

class AbstractMessageConsumer : public AbstractMessageListener, public ::core::CoreObject
{
  public:
  AbstractMessageConsumer(const std::string &name, const MessageTypeSet &relevantMessageTypes);
  ~AbstractMessageConsumer() override = default;
};

} // namespace bsgalone::core
