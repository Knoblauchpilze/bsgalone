
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource(const Repositories &repositories)
  : utils::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

void PlayerDataSource::initialize(const Uuid systemDbId,
                                  Coordinator &coordinator,
                                  DatabaseEntityMapper &entityMapper) const
{
  const auto players = m_repositories.playerRepository->findAllBySystem(systemDbId);
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

  entityMapper.registerPlayer(playerDbId, playerEntityId);
}

} // namespace bsgo
