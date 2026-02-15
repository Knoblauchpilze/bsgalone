
#pragma once

#include "IMessageListener.hh"
#include <unordered_set>

namespace bsgo {

class AbstractMessageListener : public bsgalone::core::IMessageListener
{
  public:
  AbstractMessageListener(
    const std::unordered_set<bsgalone::core::MessageType> &relevantMessageTypes);
  ~AbstractMessageListener() override = default;

  bool isMessageRelevant(const bsgalone::core::MessageType &type) const override;

  auto relevantMessageTypes() const -> std::unordered_set<bsgalone::core::MessageType>;

  private:
  std::unordered_set<bsgalone::core::MessageType> m_relevantMessageTypes{};
};

} // namespace bsgo
