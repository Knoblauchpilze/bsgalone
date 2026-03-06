
#include "TargetListMessageConsumer.hh"
#include "TargetListMessage.hh"

namespace bsgalone::client {

TargetListMessageConsumer::TargetListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                                     core::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("target", core::MessageType::TARGET_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void TargetListMessageConsumer::onMessageReceivedInternal(const core::IMessage &message)
{
  const auto targetsList = message.as<core::TargetListMessage>();

  for (const auto &targetData : targetsList.getTargetsData())
  {
    registerTarget(targetData);
  }
}

namespace {
auto tryGetEntityId(const core::DatabaseEntityMapper &entityMapper,
                    const core::Uuid dbId,
                    const core::EntityKind kind) -> std::optional<core::Uuid>
{
  switch (kind)
  {
    case core::EntityKind::SHIP:
      return entityMapper.tryGetShipEntityId(dbId);
    case core::EntityKind::OUTPOST:
      return entityMapper.tryGetOutpostEntityId(dbId);
    case core::EntityKind::ASTEROID:
      return entityMapper.tryGetAsteroidEntityId(dbId);
    default:
      throw std::invalid_argument("Failed to get entity with unsupported kind " + core::str(kind));
  }
}
} // namespace

void TargetListMessageConsumer::registerTarget(const core::Target &data) const
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

} // namespace bsgalone::client
