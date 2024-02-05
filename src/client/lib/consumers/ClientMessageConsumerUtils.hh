
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"

namespace pge {

void createMessageConsumers(bsgo::IMessageQueue &inputMessagesQueue,
                            const bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator);

}
