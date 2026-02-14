
#pragma once

#include "IMessage.hh"
#include <memory>

namespace bsgo {

class IMessageListener
{
  public:
  virtual ~IMessageListener() = default;

  virtual bool isMessageRelevant(const MessageType &type) const           = 0;
  virtual void onMessageReceived(const bsgalone::core::IMessage &message) = 0;
};

using IMessageListenerPtr = std::unique_ptr<IMessageListener>;

} // namespace bsgo
