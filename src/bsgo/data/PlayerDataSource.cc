
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource(const Repositories &repositories, const Uuid systemDbId)
  : utils::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

void PlayerDataSource::initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  const auto players = m_repositories.playerRepository->findAllBySystem(m_systemDbId);
  for (const auto &id : players)
  {
    registerPlayer(coordinator, id, entityMapper);
  }
}

void PlayerDataSource::registerPlayer(Coordinator &coordinator,
                                      const Uuid playerDbId,
                                      DatabaseEntityMapper &entityMapper) const
{
  const auto playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto player = m_repositories.playerRepository->findOneById(playerDbId);
  coordinator.addName(playerEntityId, player.name);
  coordinator.addDbId(playerEntityId, playerDbId);

  const std::unordered_set<ComponentType> toSync{ComponentType::RESOURCE};
  coordinator.addNetwork(playerEntityId, toSync);

  entityMapper.registerPlayer(playerDbId, playerEntityId);

  registerResources(coordinator, playerEntityId, playerDbId);
}

void PlayerDataSource::registerResources(Coordinator &coordinator,
                                         const Uuid playerEntityId,
                                         const Uuid playerDbId) const
{
  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
  for (const auto &resource : resources)
  {
    coordinator.addResourceComponent(playerEntityId, resource.resource, resource.amount);
  }
}

} // namespace bsgo
