
#pragma once

#include "IMessage.hh"
#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <mutex>
#include <vector>

namespace bsgo {

class SynchronizedMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  SynchronizedMessageQueue(IMessageQueuePtr messageQueue);
  ~SynchronizedMessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  std::mutex m_locker{};
  IMessageQueuePtr m_messageQueue{};
};

} // namespace bsgo
