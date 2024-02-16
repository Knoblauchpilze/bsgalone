
#include "NetworkSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<NetworkComponent>() && ent.exists<OwnerComponent>();
}
} // namespace

NetworkSystem::NetworkSystem(const Repositories &repositories)
  : AbstractSystem(SystemType::NETWORK, isEntityRelevant)
  , m_repositories(repositories)
{}

void NetworkSystem::updateEntity(Entity &entity,
                                 Coordinator &coordinator,
                                 const float /*elapsedSeconds*/) const
{
  const auto networkComp = entity.networkComp();
  if (!networkComp.needsSync())
  {
    return;
  }

  syncEntity(entity, coordinator);
}

namespace {
auto generateOutboundMessage(const Entity &entity, const Coordinator &coordinator)
  -> std::unique_ptr<ComponentSyncMessage>
{
  const auto entityDbId = entity.dbComp().dbId();
  const auto entityKind = entity.kind->kind();

  const auto owner     = coordinator.getEntity(entity.ownerComp().owner());
  const auto ownerDbId = owner.dbComp().dbId();

  return std::make_unique<ComponentSyncMessage>(entityDbId, entityKind, ownerDbId);
}
} // namespace

void NetworkSystem::syncEntity(Entity &entity, const Coordinator &coordinator) const
{
  auto &networkComp  = entity.networkComp();
  const auto &toSync = networkComp.componentsToSync();

  auto message = generateOutboundMessage(entity, coordinator);

  bool somethingToSync{false};
  for (const auto &comp : toSync)
  {
    somethingToSync |= syncComponent(entity, comp, *message);
  }

  networkComp.markForSync(false);

  if (somethingToSync)
  {
    pushMessage(std::move(message));
  }
}

bool NetworkSystem::syncComponent(const Entity &entity,
                                  const ComponentType &type,
                                  ComponentSyncMessage &message) const
{
  switch (type)
  {
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

bool NetworkSystem::syncStatusComponent(const Entity &entity, ComponentSyncMessage &message) const
{
  if (!entity.exists<StatusComponent>())
  {
    return false;
  }

  message.setStatus(entity.statusComp().status());
  return true;
}

bool NetworkSystem::syncTransformComponent(const Entity & /*entity*/,
                                           ComponentSyncMessage & /*message*/) const
{
  /// TODO: Handle velocity syncing.
  warn("Should handle transform syncing");
  return false;
}

bool NetworkSystem::syncVelocityComponent(const Entity &entity,
                                          ComponentSyncMessage & /*message*/) const
{
  if (!entity.exists<VelocityComponent>())
  {
    return false;
  }

  /// TODO: Handle velocity syncing.
  warn("Should handle velocity syncing");
  return false;
}

} // namespace bsgo
