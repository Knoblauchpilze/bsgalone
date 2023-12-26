
#include "NetworkSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<NetworkComponent>();
}
} // namespace

NetworkSystem::NetworkSystem(const Repositories &repositories)
  : AbstractSystem("network", isEntityRelevant)
  , m_repositories(repositories)
{}

void NetworkSystem::setPlayerDbId(const Uuid &playerDbId)
{
  m_playerDbId = playerDbId;
}

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
    case ComponentType::RESOURCE:
      syncResourceComponents(entity);
      break;
    default:
      error("Failed to sync component " + str(type), "Unsupported component type");
      break;
  }
}

namespace {
void syncResource(const Uuid &playerDbId,
                  const ResourceComponent &comp,
                  PlayerResourceRepository &repository)
{
  PlayerResource data{.player = playerDbId, .resource = comp.resource(), .amount = comp.amount()};
  repository.save(data);
}
} // namespace

void NetworkSystem::syncResourceComponents(Entity &entity) const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist");
  }

  for (const auto &resourceComp : entity.resources)
  {
    syncResource(*m_playerDbId, *resourceComp, *m_repositories.playerResourceRepository);
  }
}

} // namespace bsgo
