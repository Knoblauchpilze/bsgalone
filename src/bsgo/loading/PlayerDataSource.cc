
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource()
  : core::CoreObject("bsgo")
{
  setService("data");
  addModule("player");
}

PlayerDataSource::PlayerDataSource(const Repositories &repositories)
  : core::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

void PlayerDataSource::initialize(const Uuid systemDbId,
                                  Coordinator &coordinator,
                                  DatabaseEntityMapper &entityMapper) const
{
  if (!m_repositories)
  {
    error("Failed to initialize outpost", "Repositories are not set");
  }

  const auto players = m_repositories->playerRepository->findAllUndockedBySystem(systemDbId);
  for (const auto &id : players)
  {
    registerPlayer(coordinator, id, entityMapper);
  }
}

void PlayerDataSource::registerPlayer(Coordinator &coordinator,
                                      const PlayerData &data,
                                      DatabaseEntityMapper &entityMapper) const
{
  const auto maybeEntityId = entityMapper.tryGetPlayerEntityId(data.dbId);
  if (maybeEntityId)
  {
    debug("Player " + str(data.dbId) + " is already attached to entity " + str(*maybeEntityId));
    return;
  }

  const auto playerEntityId = coordinator.createEntity(bsgalone::core::EntityKind::PLAYER);

  coordinator.addDbId(playerEntityId, data.dbId);
  coordinator.addName(playerEntityId, data.name);

  entityMapper.registerPlayer(data.dbId, playerEntityId);
}

void PlayerDataSource::registerPlayer(Coordinator &coordinator,
                                      const Uuid playerDbId,
                                      DatabaseEntityMapper &entityMapper) const
{
  if (!m_repositories)
  {
    error("Failed to initialize outpost", "Repositories are not set");
  }

  const auto data = m_repositories->playerRepository->findOneById(playerDbId);

  PlayerData out{
    .dbId = data.id,
    .name = data.name,
  };

  registerPlayer(coordinator, out, entityMapper);
}

} // namespace bsgo
