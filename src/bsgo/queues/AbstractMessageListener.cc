
#include "AbstractMessageListener.hh"

namespace bsgo {

AbstractMessageListener::AbstractMessageListener(
  const std::unordered_set<bsgalone::core::MessageType> &relevantMessageTypes)
  : IMessageListener()
  , m_relevantMessageTypes(relevantMessageTypes)
{}

bool AbstractMessageListener::isMessageRelevant(const bsgalone::core::MessageType &type) const
{
  return m_relevantMessageTypes.contains(type);
}

auto AbstractMessageListener::relevantMessageTypes() const
  -> std::unordered_set<bsgalone::core::MessageType>
{
  return m_relevantMessageTypes;
}

} // namespace bsgo
