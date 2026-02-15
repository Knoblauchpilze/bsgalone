
#pragma once

#include "IMessageQueue.hh"
#include <unordered_map>

namespace bsgo {

class AbstractMessageQueue : public bsgalone::core::IMessageQueue
{
  public:
  AbstractMessageQueue();
  ~AbstractMessageQueue() override = default;

  void addListener(bsgalone::core::IMessageListenerPtr listener) override;

  protected:
  std::vector<bsgalone::core::IMessageListenerPtr> m_listeners{};
  std::unordered_multimap<bsgalone::core::MessageType, bsgalone::core::IMessageListener *>
    m_listenersTable{};
};

} // namespace bsgo
