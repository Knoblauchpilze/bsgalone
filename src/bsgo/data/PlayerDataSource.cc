
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource(const Repositories &repositories,
                                   const Uuid systemDbId,
                                   const std::optional<Uuid> &playerDbId)
  : utils::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
  , m_playerDbId(playerDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

auto PlayerDataSource::initialize(Coordinator &coordinator) const -> std::optional<Uuid>
{
  const auto players = m_repositories.playerRepository->findAllBySystem(m_systemDbId);
  for (const auto &id : players)
  {
    registerPlayer(coordinator, id);
  }

  return m_playerEntityId;
}

void PlayerDataSource::registerPlayer(Coordinator &coordinator, const Uuid playerId) const
{
  const auto playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto player = m_repositories.playerRepository->findOneById(playerId);
  coordinator.addName(playerEntityId, player.name);
  coordinator.addDbId(playerEntityId, playerId);

  const std::unordered_set<ComponentType> toSync{ComponentType::RESOURCE};
  coordinator.addNetwork(playerEntityId, toSync);

  if (m_playerDbId && playerId == *m_playerDbId)
  {
    m_playerEntityId = playerEntityId;
  }
}

} // namespace bsgo
