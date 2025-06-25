
#include "LoadingService.hh"

namespace bsgo {

LoadingService::LoadingService(const Repositories &repositories)
  : AbstractService("loading", repositories)
{}

auto LoadingService::getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>
{
  const auto playerIds = m_repositories.playerRepository->findAllBySystem(systemDbId);

  std::vector<Player> players{};

  for (const auto &playerId : playerIds)
  {
    const auto player = m_repositories.playerRepository->findOneById(playerId);
    players.emplace_back(player);
  }

  return players;
}

auto LoadingService::getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>
{
  const auto asteroidIds = m_repositories.systemRepository->findAllAsteroidsBySystem(systemDbId);

  std::vector<AsteroidProps> asteroids{};

  for (const auto &asteroidId : asteroidIds)
  {
    const auto asteroid = m_repositories.asteroidRepository->findOneById(asteroidId);

    std::optional<Uuid> resource{};
    std::optional<float> amount{};
    if (asteroid.loot)
    {
      const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidId);
      resource        = loot.resource;
      amount          = loot.amount;
    }

    asteroids.emplace_back(asteroidId, asteroid, resource, amount);
  }

  return asteroids;
}

auto LoadingService::getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>
{
  const auto outpostIds = m_repositories.systemRepository->findAllOutpostsBySystem(systemDbId);

  std::vector<OutpostProps> outposts{};

  for (const auto &outpostId : outpostIds)
  {
    const auto outpost = m_repositories.systemOutpostRepository->findOneById(outpostId);

    // TODO: the target id is left empty for now. We should give this service
    // access to the coordinator and the entity mapper and try to fetch it
    // from there. Probably in a dedicated method.
    outposts.emplace_back(outpostId, outpost);
  }

  return outposts;
}

} // namespace bsgo
