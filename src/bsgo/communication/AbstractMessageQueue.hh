
#pragma once

#include "IMessageQueue.hh"
#include <unordered_map>

namespace bsgo {

class AbstractMessageQueue : public IMessageQueue
{
  public:
  AbstractMessageQueue();
  ~AbstractMessageQueue() override = default;

  void addListener(IMessageListener *listener) override;

  protected:
  std::unordered_multimap<MessageType, IMessageListener *> m_listeners{};
};

} // namespace bsgo
