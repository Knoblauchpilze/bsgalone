
#pragma once

#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <vector>

namespace bsgo {

class MessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  MessageQueue();
  ~MessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;

  private:
  std::vector<IMessagePtr> m_messages{};
};

} // namespace bsgo
