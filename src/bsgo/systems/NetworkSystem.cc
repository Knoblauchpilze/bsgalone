
#include "NetworkSystem.hh"
#include "ComponentSyncMessage.hh"

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
                                 const float /*elapsedSeconds*/) const
{
  const auto networkComp = entity.networkComp();
  if (!networkComp.needsSync())
  {
    return;
  }

  syncEntity(entity);
}

void NetworkSystem::syncEntity(Entity &entity) const
{
  auto &networkComp  = entity.networkComp();
  const auto &toSync = networkComp.componentsToSync();

  for (const auto &comp : toSync)
  {
    syncComponent(entity, comp);
  }

  networkComp.markForSync(false);
}

void NetworkSystem::syncComponent(Entity &entity, const ComponentType &type) const
{
  switch (type)
  {
    case ComponentType::STATUS:
      syncStatusComponent(entity);
      break;
    default:
      error("Failed to sync component " + str(type), "Unsupported component type");
      break;
  }
}

void NetworkSystem::syncStatusComponent(Entity &entity) const
{
  if (!entity.exists<StatusComponent>())
  {
    return;
  }

  /// TODO: Handle sync of status.
  warn("Should sync status component");
}

} // namespace bsgo
