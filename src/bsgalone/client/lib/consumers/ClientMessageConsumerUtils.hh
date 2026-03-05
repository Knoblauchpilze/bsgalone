
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"

namespace pge {

void createMessageConsumers(bsgalone::core::IMessageQueue &inputMessagesQueue,
                            bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator);

}
