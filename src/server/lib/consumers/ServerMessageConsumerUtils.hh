
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue &inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services);

}
