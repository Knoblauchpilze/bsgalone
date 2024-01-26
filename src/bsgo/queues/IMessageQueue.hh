
#pragma once

#include "IMessage.hh"
#include "IMessageListener.hh"
#include <memory>

namespace bsgo {

class IMessageQueue
{
  public:
  virtual ~IMessageQueue() = default;

  virtual void pushMessage(IMessagePtr message)          = 0;
  virtual void addListener(IMessageListenerPtr listener) = 0;
  virtual bool empty()                                   = 0;

  virtual void processMessages(const std::optional<int> &amount = {}) = 0;
};

using IMessageQueuePtr = std::unique_ptr<IMessageQueue>;

} // namespace bsgo
