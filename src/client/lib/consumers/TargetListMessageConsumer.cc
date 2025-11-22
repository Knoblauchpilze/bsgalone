
#include "TargetListMessageConsumer.hh"
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

namespace {
auto tryGetEntityId(const bsgo::DatabaseEntityMapper &entityMapper,
                    const bsgo::Uuid dbId,
                    const bsgo::EntityKind kind) -> std::optional<bsgo::Uuid>
{
  switch (kind)
  {
    case bsgo::EntityKind::SHIP:
      return entityMapper.tryGetShipEntityId(dbId);
    case bsgo::EntityKind::OUTPOST:
      return entityMapper.tryGetOutpostEntityId(dbId);
    default:
      throw std::invalid_argument("Failed to get entity with unsupported kind " + bsgo::str(kind));
  }
}
} // namespace

void TargetListMessageConsumer::registerTarget(const bsgo::TargetData &data) const
{
  const auto maybeSourceEntityId = tryGetEntityId(m_entityMapper, data.sourceDbId, data.sourceKind);
  if (!maybeSourceEntityId)
  {
    error("Failed to register target, failed to find source entity: " + bsgo::str(data.sourceKind)
          + " " + bsgo::str(data.sourceDbId));
  }

  const auto maybeTargetEntityId = tryGetEntityId(m_entityMapper, data.targetDbId, data.targetKind);
  if (!maybeTargetEntityId)
  {
    error("Failed to register target, failed to find target entity: " + bsgo::str(data.targetDbId)
          + " " + bsgo::str(data.targetDbId));
  }

  auto source = m_coordinator->getEntity(*maybeSourceEntityId);
  source.targetComp().setTarget(*maybeTargetEntityId);

  const auto target = m_coordinator->getEntity(*maybeTargetEntityId);
  debug("Registering target " + target.str() + " for " + source.str());
}

} // namespace pge
