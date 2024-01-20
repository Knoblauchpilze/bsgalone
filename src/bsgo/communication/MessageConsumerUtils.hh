
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include <vector>

namespace bsgo {

auto registerAllConsumersToQueue(IMessageQueue *const inputMessagesQueue,
                                 IMessageQueue *const outputMessagesQueue,
                                 const Services &services) -> std::vector<IMessageListenerPtr>;

}
