
#pragma once

#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

void createMessageConsumers(bsgalone::core::IMessageQueue *const inputMessagesQueue,
                            bsgalone::core::IMessageQueue *const outputMessagesQueue,
                            const Services &services);

}
