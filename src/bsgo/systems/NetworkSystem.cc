
#include "NetworkSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<NetworkComponent>();
}
} // namespace

NetworkSystem::NetworkSystem(const Repositories &repositories)
  : AbstractSystem(SystemType::NETWORK, isEntityRelevant)
  , m_repositories(repositories)
{}

void NetworkSystem::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const float elapsedSeconds) const
{
  auto &networkComp = entity.networkComp();
  networkComp.update(elapsedSeconds);

  if (!networkComp.needsSync())
  {
    return;
  }

  syncEntity(entity);
}

namespace {
auto generateOutboundMessage(const Entity &entity) -> std::unique_ptr<ComponentSyncMessage>
{
  const auto entityDbId = entity.dbComp().dbId();
  const auto entityKind = entity.kind->kind();

  return std::make_unique<ComponentSyncMessage>(entityDbId, entityKind);
}
} // namespace

void NetworkSystem::syncEntity(Entity &entity) const
{
  auto &networkComp  = entity.networkComp();
  const auto &toSync = networkComp.componentsToSync();

  auto message = generateOutboundMessage(entity);

  bool somethingToSync{false};
  for (const auto &comp : toSync)
  {
    somethingToSync |= syncComponent(entity, comp, *message);
  }

  networkComp.markAsJustSynced();

  if (somethingToSync)
  {
    pushInternalMessage(std::move(message));
  }
}

bool NetworkSystem::syncComponent(const Entity &entity,
                                  const ComponentType &type,
                                  ComponentSyncMessage &message) const
{
  switch (type)
  {
    case ComponentType::HEALTH:
      return syncHealthComponent(entity, message);
      break;
    case ComponentType::POWER:
      return syncPowerComponent(entity, message);
      break;
    case ComponentType::STATUS:
      return syncStatusComponent(entity, message);
      break;
    case ComponentType::TRANSFORM:
      return syncTransformComponent(entity, message);
      break;
    case ComponentType::VELOCITY:
      return syncVelocityComponent(entity, message);
      break;
    default:
      error("Failed to sync component " + str(type), "Unsupported component type");
      break;
  }

  // Useless because of the error above.
  return false;
}

bool NetworkSystem::syncHealthComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  if (!entity.exists<HealthComponent>())
  {
    return false;
  }

  message.setHealth(entity.healthComp().value());
  return true;
}

bool NetworkSystem::syncPowerComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  if (!entity.exists<PowerComponent>())
  {
    return false;
  }

  message.setPower(entity.powerComp().value());
  return true;
}

bool NetworkSystem::syncStatusComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  if (!entity.exists<StatusComponent>())
  {
    return false;
  }

  message.setStatus(entity.statusComp().status());
  return true;
}

bool NetworkSystem::syncTransformComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  message.setPosition(entity.transformComp().position());
  return true;
}

bool NetworkSystem::syncVelocityComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  if (!entity.exists<VelocityComponent>())
  {
    return false;
  }

  const auto &velocityComp = entity.velocityComp();
  message.setSpeed(velocityComp.speed());
  message.setAcceleration(velocityComp.acceleration());

  return true;
}

} // namespace bsgo
