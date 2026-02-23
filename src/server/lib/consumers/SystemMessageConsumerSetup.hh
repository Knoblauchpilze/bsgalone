
#pragma once

#include "IMessageQueue.hh"
#include "Services.hh"
#include "SystemQueues.hh"

namespace bsgo {

void createSystemMessageConsumers(bsgalone::core::IMessageQueue &inputMessagesQueue,
                                  SystemQueueMap systemQueues,
                                  bsgalone::core::IMessageQueue *const outputMessagesQueue);

}
