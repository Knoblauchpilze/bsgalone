
#pragma once

#include "AbstractMessageQueue.hh"
#include "IMessage.hh"
#include <core_utils/CoreObject.hh>
#include <deque>
#include <mutex>
#include <vector>

namespace bsgo {

class SynchronizedMessageQueue : public AbstractMessageQueue, public utils::CoreObject
{
  public:
  SynchronizedMessageQueue(const std::string &name);
  ~SynchronizedMessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  std::mutex m_locker{};
  std::deque<IMessagePtr> m_messages{};

  void processMessage(const IMessage &message) const;
};

} // namespace bsgo
