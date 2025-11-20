
#include "TargetListMessageConsumer.hh"
#include "OutpostDataSource.hh"
#include "TargetListMessage.hh"

namespace pge {

TargetListMessageConsumer::TargetListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                     bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("target", {bsgo::MessageType::TARGET_LIST})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void TargetListMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto targetsList = message.as<bsgo::TargetListMessage>();

  for (const auto &targetData : targetsList.getTargetsData())
  {
    registerTarget(targetData);
  }
}

void TargetListMessageConsumer::registerTarget(const bsgo::TargetData & /*data*/) const
{
  // TODO: Handle registering the target
  warn("should register target");
}

} // namespace pge
