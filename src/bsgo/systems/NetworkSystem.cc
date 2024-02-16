
#include "NetworkSystem.hh"
#include "ComponentSyncMessage.hh"
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

void NetworkSystem::syncEntity(Entity &entity, const Coordinator &coordinator) const
{
  auto &networkComp  = entity.networkComp();
  const auto &toSync = networkComp.componentsToSync();

  for (const auto &comp : toSync)
  {
    syncComponent(entity, comp, coordinator);
  }

  networkComp.markForSync(false);
}

void NetworkSystem::syncComponent(const Entity &entity,
                                  const ComponentType &type,
                                  const Coordinator &coordinator) const
{
  switch (type)
  {
    case ComponentType::STATUS:
      syncStatusComponent(entity, coordinator);
      break;
    default:
      error("Failed to sync component " + str(type), "Unsupported component type");
      break;
  }
}

void NetworkSystem::syncStatusComponent(const Entity &entity, const Coordinator &coordinator) const
{
  if (!entity.exists<StatusComponent>() || !entity.exists<OwnerComponent>())
  {
    return;
  }

  const auto entityDbId = entity.dbComp().dbId();
  const auto entityKind = entity.kind->kind();

  const auto owner     = coordinator.getEntity(entity.ownerComp().owner());
  const auto ownerDbId = owner.dbComp().dbId();

  debug("pushing sync component message");
  pushMessage(std::make_unique<ComponentSyncMessage>(entityDbId, entityKind, ownerDbId));
}

} // namespace bsgo
