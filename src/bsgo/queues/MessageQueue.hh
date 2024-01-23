
#pragma once

#include "AbstractMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <deque>

namespace bsgo {

class MessageQueue : public AbstractMessageQueue, public utils::CoreObject
{
  public:
  MessageQueue();
  ~MessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  std::deque<IMessagePtr> m_messages{};

  void processMessage(const IMessage &message) const;
};

} // namespace bsgo
