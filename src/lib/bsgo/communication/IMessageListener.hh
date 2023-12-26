
#pragma once

#include "IMessage.hh"

namespace bsgo {

class IMessageListener
{
  public:
  virtual ~IMessageListener() = default;

  virtual bool isMessageRelevant(const MessageType &type) const = 0;
  virtual void onMessageReceived(const IMessage &message)       = 0;
};

} // namespace bsgo
