
#pragma once

#include "IMessageListener.hh"
#include <unordered_set>

namespace bsgo {

class AbstractMessageListener : public IMessageListener
{
  public:
  AbstractMessageListener(const std::unordered_set<MessageType> &relevantMessageTypes);
  ~AbstractMessageListener() override = default;

  bool isMessageRelevant(const MessageType &type) const override;

  private:
  std::unordered_set<MessageType> m_relevantMessageTypes{};
};

} // namespace bsgo
