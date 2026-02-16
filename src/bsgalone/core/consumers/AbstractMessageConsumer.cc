
#include "AbstractMessageConsumer.hh"

namespace bsgalone::core {

AbstractMessageConsumer::AbstractMessageConsumer(const std::string &name,
                                                 const MessageTypeSet &relevantMessageTypes)
  : AbstractMessageListener(relevantMessageTypes)
  , ::core::CoreObject(name)
{
  setService("consumer");
}

} // namespace bsgalone::core
