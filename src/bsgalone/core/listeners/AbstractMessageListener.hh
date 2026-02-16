
#pragma once

#include "IMessageListener.hh"
#include <unordered_set>

namespace bsgalone::core {

class AbstractMessageListener : public IMessageListener
{
  public:
  using MessageTypeSet = std::unordered_set<MessageType>;

  AbstractMessageListener(const MessageTypeSet &relevantMessageTypes);
  ~AbstractMessageListener() override = default;

  bool isMessageRelevant(const MessageType &type) const override;

  auto relevantMessageTypes() const -> MessageTypeSet;

  private:
  MessageTypeSet m_relevantMessageTypes{};
};

} // namespace bsgalone::core
