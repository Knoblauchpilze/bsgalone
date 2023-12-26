
#pragma once

#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>
#include <vector>

namespace bsgo {

class MessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  MessageQueue();
  ~MessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(const MessageType &messageType, IMessageListener *listener) override;

  void processMessages() override;

  private:
  std::vector<IMessagePtr> m_messages{};
  std::unordered_multimap<MessageType, IMessageListener *> m_listeners{};

  void processMessage(const IMessage &message) const;
};

} // namespace bsgo
