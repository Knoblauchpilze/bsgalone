
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const Uuid &playerId,
                       const Uuid &playerShipId,
                       const CoordinatorShPtr &coordinator,
                       const Repositories &repositories)
  : IView("player", coordinator, repositories)
  , m_playerId(playerId)
  , m_playerShipId(playerShipId)
{}

auto PlayerView::getPlayerResources() const -> std::vector<PlayerResource>
{
  return m_repositories.playerResourceRepository->findAllByPlayer(m_playerId);
}

auto PlayerView::getPlayerWeapons() const -> std::vector<PlayerWeapon>
{
  const auto shipWeapons = m_repositories.shipWeaponRepository->findAllByShip(m_playerShipId);
  auto ids               = m_repositories.playerWeaponRepository->findAllByPlayer(m_playerId);
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
  const auto usedIds = m_repositories.shipComputerRepository->findAllByShip(m_playerShipId);
  auto ids           = m_repositories.playerComputerRepository->findAllByPlayer(m_playerId);
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
  const auto ids = m_repositories.playerShipRepository->findAllByPlayer(m_playerId);

  std::vector<PlayerShip> out;
  for (const auto &id : ids)
  {
    const auto ship = m_repositories.playerShipRepository->findOneById(id);
    out.push_back(ship);
  }

  return out;
}

auto PlayerView::tryLogin(const std::string &name, const std::string &password) const
  -> std::optional<Uuid>
{
  const auto maybePlayer = m_repositories.playerRepository->findOneByName(name);
  if (!maybePlayer)
  {
    warn("No player with name \"" + name + "\"");
    return {};
  }
  if (maybePlayer->password != password)
  {
    warn("Wrong password for player \"" + name + "\"",
         "Expected " + maybePlayer->password + " but got " + password);
    return {};
  }

  return maybePlayer->id;
}

} // namespace bsgo
