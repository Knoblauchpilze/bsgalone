
#include "AbstractMessageConsumer.hh"

namespace bsgo {

AbstractMessageConsumer::AbstractMessageConsumer(
  const std::string &name,
  const std::unordered_set<MessageType> &relevantMessageTypes)
  : AbstractMessageListener(relevantMessageTypes)
  , utils::CoreObject(name)
{
  setService("consumer");
}

} // namespace bsgo
