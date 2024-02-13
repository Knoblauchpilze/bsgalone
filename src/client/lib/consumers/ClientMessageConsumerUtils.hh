
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"

namespace pge {

void createMessageConsumers(const bsgo::Repositories &repositories,
                            bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator);

}
