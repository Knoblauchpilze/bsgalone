
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include <vector>

namespace bsgo {

auto registerAllConsumersToQueue(IMessageQueue *const messageQueue, const Services &services)
  -> std::vector<IMessageListenerPtr>;

}
