
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue *const inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services);

}
