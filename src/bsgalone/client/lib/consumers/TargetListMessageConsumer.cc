
#include "TargetListMessageConsumer.hh"
#include "TargetListMessage.hh"

namespace pge {

TargetListMessageConsumer::TargetListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                     bsgo::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("target", bsgalone::core::MessageType::TARGET_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void TargetListMessageConsumer::onMessageReceivedInternal(const bsgalone::core::IMessage &message)
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
                    const bsgalone::core::EntityKind kind) -> std::optional<bsgo::Uuid>
{
  switch (kind)
  {
    case bsgalone::core::EntityKind::SHIP:
      return entityMapper.tryGetShipEntityId(dbId);
    case bsgalone::core::EntityKind::OUTPOST:
      return entityMapper.tryGetOutpostEntityId(dbId);
    case bsgalone::core::EntityKind::ASTEROID:
      return entityMapper.tryGetAsteroidEntityId(dbId);
    default:
      throw std::invalid_argument("Failed to get entity with unsupported kind "
                                  + bsgalone::core::str(kind));
  }
}
} // namespace

void TargetListMessageConsumer::registerTarget(const bsgalone::core::Target &data) const
{
  if (!data.targetDbId || !data.targetKind)
  {
    error("Received unexpected empty target when loading targets for " + data.str());
  }

  const auto maybeSourceEntityId = tryGetEntityId(m_entityMapper, data.sourceDbId, data.sourceKind);
  if (!maybeSourceEntityId)
  {
    error("Failed to register target, failed to find source entity for target " + data.str());
  }

  const auto maybeTargetEntityId = tryGetEntityId(m_entityMapper,
                                                  *data.targetDbId,
                                                  *data.targetKind);
  if (!maybeTargetEntityId)
  {
    error("Failed to register target, failed to find target entity for target " + data.str());
  }

  auto source = m_coordinator->getEntity(*maybeSourceEntityId);
  source.targetComp().setTarget(*maybeTargetEntityId);

  const auto target = m_coordinator->getEntity(*maybeTargetEntityId);
  debug("Registering target " + target.str() + " for " + source.str());
}

} // namespace pge
