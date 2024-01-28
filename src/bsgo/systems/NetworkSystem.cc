
#include "NetworkSystem.hh"
#include "SlotComponentMessage.hh"

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
    case ComponentType::RESOURCE:
      syncResourceComponents(entity);
      break;
    case ComponentType::COMPUTER_SLOT:
      syncComputerSlotComponents(entity);
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
  if (!entity.exists<OwnerComponent>())
  {
    return;
  }

  const auto playerDbId = entity.dbComp().dbId();
  for (const auto &resourceComp : entity.resources)
  {
    syncResource(playerDbId, *resourceComp, *m_repositories.playerResourceRepository);
  }
}

void NetworkSystem::syncComputerSlotComponents(Entity &entity) const
{
  auto count = static_cast<int>(entity.computers.size());
  for (auto id = 0; id < count; ++id)
  {
    auto message = std::make_unique<SlotComponentMessage>(entity.uuid, id, *entity.computers[id]);
    pushMessage(std::move(message));
  }
}

} // namespace bsgo
