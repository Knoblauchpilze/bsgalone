
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include <vector>

namespace bsgo {

void registerAllConsumersToQueue(IMessageQueue &inputMessagesQueue,
                                 IMessageQueue *const outputMessagesQueue,
                                 const Services &services);

}
