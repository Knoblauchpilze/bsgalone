
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource(const Repositories &repositories, const Uuid &playerDbId)
  : utils::CoreObject("bsgo")
  , m_playerDbId(playerDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

auto PlayerDataSource::initialize(Coordinator &coordinator) const -> Uuid
{
  const auto playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto player = m_repositories.playerRepository->findOneById(m_playerDbId);
  coordinator.addName(playerEntityId, player.name);
  coordinator.addDbId(playerEntityId, m_playerDbId);

  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(m_playerDbId);
  for (const auto &resource : resources)
  {
    coordinator.addResourceComponent(playerEntityId, resource.resource, resource.amount);
  }

  const std::unordered_set<ComponentType> toSync{ComponentType::RESOURCE};
  coordinator.addNetwork(playerEntityId, toSync);

  return playerEntityId;
}

} // namespace bsgo
