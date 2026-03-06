
#pragma once

#include "IMessageQueue.hh"
#include "Services.hh"
#include "SystemQueues.hh"

namespace bsgalone::server {

void createSystemMessageConsumers(core::IMessageQueue &inputMessagesQueue,
                                  SystemQueueMap systemQueues,
                                  core::IMessageQueue *const outputMessagesQueue);

}
