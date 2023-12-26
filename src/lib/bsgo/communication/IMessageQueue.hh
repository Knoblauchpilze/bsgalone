
#pragma once

#include "IMessage.hh"
#include "IMessageListener.hh"
#include <memory>

namespace bsgo {

class IMessageQueue
{
  public:
  virtual ~IMessageQueue() = default;

  virtual void pushMessage(IMessagePtr message)                                        = 0;
  virtual void addListener(const MessageType &messageType, IMessageListener *listener) = 0;

  virtual void processMessages() = 0;
};

} // namespace bsgo
