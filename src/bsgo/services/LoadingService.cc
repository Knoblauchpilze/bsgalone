
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

} // namespace bsgo
