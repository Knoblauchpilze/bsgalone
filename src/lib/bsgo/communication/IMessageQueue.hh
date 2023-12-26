
#pragma once

#include "IMessage.hh"
#include <memory>

namespace bsgo {

class IMessageQueue
{
  public:
  virtual ~IMessageQueue() = default;

  virtual void pushMessage(IMessagePtr message) = 0;
};

} // namespace bsgo
