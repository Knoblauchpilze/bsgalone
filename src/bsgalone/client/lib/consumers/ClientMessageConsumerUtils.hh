
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"

namespace bsgalone::client {

void createMessageConsumers(core::IMessageQueue &inputMessagesQueue,
                            core::DatabaseEntityMapper &entityMapper,
                            const core::CoordinatorShPtr &coordinator);

}
