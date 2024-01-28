
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

void createMessageConsumers(bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::IMessageQueue *const outputMessagesQueue,
                            const bsgo::Services &services);

}
