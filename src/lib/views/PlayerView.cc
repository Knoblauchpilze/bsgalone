
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("player", coordinator, repositories)
{}

void PlayerView::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

void PlayerView::setPlayerShipDbId(const Uuid &ship)
{
  m_playerShipDbId = ship;
}

bool PlayerView::isReady() const noexcept
{
  return m_playerDbId && m_playerShipDbId;
}

auto PlayerView::getPlayerResources() const -> std::vector<PlayerResource>
{
  return m_repositories.playerResourceRepository->findAllByPlayer(*m_playerDbId);
}

auto PlayerView::getPlayerWeapons() const -> std::vector<PlayerWeapon>
{
  const auto shipWeapons = m_repositories.shipWeaponRepository->findAllByShip(*m_playerShipDbId);
  auto ids               = m_repositories.playerWeaponRepository->findAllByPlayer(*m_playerDbId);
  for (const auto &shipWeapon : shipWeapons)
  {
    ids.erase(shipWeapon.id);
  }

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
  const auto usedIds = m_repositories.shipComputerRepository->findAllByShip(*m_playerShipDbId);
  auto ids           = m_repositories.playerComputerRepository->findAllByPlayer(*m_playerDbId);
  for (const auto usedId : usedIds)
  {
    ids.erase(usedId);
  }

  std::vector<PlayerComputer> out;
  for (const auto &id : ids)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(id);
    out.push_back(computer);
  }
  return out;
}

auto PlayerView::getPlayerShips() const -> std::vector<PlayerShip>
{
  const auto ids = m_repositories.playerShipRepository->findAllByPlayer(*m_playerDbId);

  std::vector<PlayerShip> out;
  for (const auto &id : ids)
  {
    const auto ship = m_repositories.playerShipRepository->findOneById(id);
    out.push_back(ship);
  }

  return out;
}

} // namespace bsgo
