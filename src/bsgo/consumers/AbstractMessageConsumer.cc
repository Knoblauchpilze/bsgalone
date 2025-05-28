
#include "AbstractMessageConsumer.hh"

namespace bsgo {

AbstractMessageConsumer::AbstractMessageConsumer(
  const std::string &name,
  const std::unordered_set<MessageType> &relevantMessageTypes)
  : AbstractMessageListener(relevantMessageTypes)
  , core::CoreObject(name)
{
  setService("consumer");
}

} // namespace bsgo
