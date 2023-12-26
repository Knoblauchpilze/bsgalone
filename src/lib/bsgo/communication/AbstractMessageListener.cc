
#include "AbstractMessageListener.hh"

namespace bsgo {

AbstractMessageListener::AbstractMessageListener(
  const std::unordered_set<MessageType> &relevantMessageTypes)
  : IMessageListener()
  , m_relevantMessageTypes(relevantMessageTypes)
{}

bool AbstractMessageListener::isMessageRelevant(const MessageType &type) const
{
  return m_relevantMessageTypes.contains(type);
}

} // namespace bsgo
