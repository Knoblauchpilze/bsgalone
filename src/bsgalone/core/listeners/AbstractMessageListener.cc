
#include "AbstractMessageListener.hh"

namespace bsgalone::core {

AbstractMessageListener::AbstractMessageListener(const MessageTypeSet &relevantMessageTypes)
  : IMessageListener()
  , m_relevantMessageTypes(relevantMessageTypes)
{}

bool AbstractMessageListener::isMessageRelevant(const MessageType &type) const
{
  return m_relevantMessageTypes.contains(type);
}

auto AbstractMessageListener::relevantMessageTypes() const -> MessageTypeSet
{
  return m_relevantMessageTypes;
}

} // namespace bsgalone::core
