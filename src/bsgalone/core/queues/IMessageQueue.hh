
#pragma once

#include "IMessage.hh"
#include "IMessageListener.hh"
#include <memory>

namespace bsgo {

class IMessageQueue
{
  public:
  virtual ~IMessageQueue() = default;

  virtual void pushMessage(bsgalone::core::IMessagePtr message) = 0;
  virtual void addListener(IMessageListenerPtr listener)        = 0;
  virtual bool empty()                                          = 0;

  virtual void processMessages() = 0;
};

using IMessageQueuePtr   = std::unique_ptr<IMessageQueue>;
using IMessageQueueShPtr = std::shared_ptr<IMessageQueue>;

} // namespace bsgo
