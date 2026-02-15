
#pragma once

#include "AbstractMessageQueue.hh"
#include "CoreObject.hh"
#include "IMessage.hh"
#include <deque>
#include <mutex>
#include <vector>

namespace bsgo {

class SynchronizedMessageQueue : public AbstractMessageQueue, public core::CoreObject
{
  public:
  SynchronizedMessageQueue(const std::string &name);
  ~SynchronizedMessageQueue() override = default;

  void pushMessage(bsgalone::core::IMessagePtr message) override;
  void addListener(bsgalone::core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  std::mutex m_locker{};
  std::deque<bsgalone::core::IMessagePtr> m_messages{};

  void processMessage(const bsgalone::core::IMessage &message) const;
};

} // namespace bsgo
