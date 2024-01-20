
#pragma once

#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <deque>
#include <unordered_map>

namespace bsgo {

class MessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  MessageQueue();
  ~MessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  std::deque<IMessagePtr> m_messages{};
  std::unordered_multimap<MessageType, IMessageListener *> m_listeners{};

  void processMessage(const IMessage &message) const;
};

} // namespace bsgo
