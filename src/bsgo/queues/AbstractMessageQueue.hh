
#pragma once

#include "IMessageQueue.hh"
#include <unordered_map>

namespace bsgo {

class AbstractMessageQueue : public IMessageQueue
{
  public:
  AbstractMessageQueue();
  ~AbstractMessageQueue() override = default;

  void addListener(IMessageListenerPtr listener) override;

  protected:
  std::vector<IMessageListenerPtr> m_listeners{};
  std::unordered_multimap<MessageType, IMessageListener *> m_listenersTable{};
};

} // namespace bsgo
