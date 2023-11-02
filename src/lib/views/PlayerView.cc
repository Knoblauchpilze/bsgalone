
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const Uuid &playerId,
                       const CoordinatorShPtr &coordinator,
                       const Repositories &repositories)
  : IView("player", coordinator, repositories)
  , m_playerId(playerId)
{}

auto PlayerView::getPlayerResources() const -> std::vector<PlayerResource>
{
  const auto ids = m_repositories.playerResourceRepository->findAllByPlayer(m_playerId);

  std::vector<PlayerResource> out;
  for (const auto &id : ids)
  {
    const auto resource = m_repositories.playerResourceRepository->findOneById(id);
    out.push_back(resource);
  }
  return out;
}

auto PlayerView::getPlayerWeapons() const -> std::vector<PlayerWeapon>
{
  const auto ids = m_repositories.playerWeaponRepository->findAllByPlayer(m_playerId);

  std::vector<PlayerWeapon> out;
  for (const auto &id : ids)
  {
    const auto weapon = m_repositories.playerWeaponRepository->findOneById(id);
    out.push_back(weapon);
  }
  return out;
}

auto PlayerView::getPlayerComputers() const -> std::vector<PlayerComputer>
{
  const auto ids = m_repositories.playerComputerRepository->findAllByPlayer(m_playerId);

  std::vector<PlayerComputer> out;
  for (const auto &id : ids)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(id);
    out.push_back(computer);
  }
  return out;
}

} // namespace bsgo
