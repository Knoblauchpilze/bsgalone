
#pragma once

#include "IMessageQueue.hh"
#include "SynchronizedMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class AsyncMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  AsyncMessageQueue();
  ~AsyncMessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;

  void processMessages() override;

  private:
  SynchronizedMessageQueue m_messageQueue{};
  std::unordered_multimap<MessageType, IMessageListener *> m_listeners{};
};

} // namespace bsgo
