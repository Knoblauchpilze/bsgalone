
#include "PlayerView.hh"
#include "HangarMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PurchaseMessage.hh"
#include "SignupMessage.hh"

namespace pge {

PlayerView::PlayerView(const bsgo::Repositories &repositories,
                       bsgo::IMessageQueue *const outputMessageQueue)
  : AbstractView("player")
  , m_repositories(repositories)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

void PlayerView::setPlayerDbId(const bsgo::Uuid player)
{
  m_playerDbId = player;
}

bool PlayerView::isReady() const noexcept
{
  return m_playerDbId.has_value();
}

auto PlayerView::getPlayerDbId() const -> bsgo::Uuid
{
  checkPlayerDbIdExists();
  return *m_playerDbId;
}

auto PlayerView::getPlayerFaction() const -> bsgo::Faction
{
  checkPlayerDbIdExists();
  const auto player = m_repositories.playerRepository->findOneById(*m_playerDbId);
  return player.faction;
}

auto PlayerView::getPlayerResources() const -> std::vector<bsgo::PlayerResource>
{
  checkPlayerDbIdExists();
  return m_repositories.playerResourceRepository->findAllByPlayer(*m_playerDbId);
}

auto PlayerView::getPlayerWeapons() const -> std::vector<bsgo::PlayerWeapon>
{
  checkPlayerDbIdExists();

  std::vector<bsgo::ShipWeapon> shipWeapons{};
  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(*m_playerDbId);
  for (const auto &shipId : ships)
  {
    const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(shipId);
    shipWeapons.insert(shipWeapons.end(), weapons.begin(), weapons.end());
  }

  auto ids = m_repositories.playerWeaponRepository->findAllByPlayer(*m_playerDbId);
  for (const auto &shipWeapon : shipWeapons)
  {
    ids.erase(shipWeapon.weapon);
  }

  std::vector<bsgo::PlayerWeapon> out;
  for (const auto &id : ids)
  {
    const auto weapon = m_repositories.playerWeaponRepository->findOneById(id);
    out.push_back(weapon);
  }
  return out;
}

auto PlayerView::getPlayerComputers() const -> std::vector<bsgo::PlayerComputer>
{
  checkPlayerDbIdExists();

  std::vector<bsgo::Uuid> shipComputers{};
  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(*m_playerDbId);
  for (const auto &shipId : ships)
  {
    const auto computers = m_repositories.shipComputerRepository->findAllByShip(shipId);
    shipComputers.insert(shipComputers.end(), computers.begin(), computers.end());
  }

  auto ids = m_repositories.playerComputerRepository->findAllByPlayer(*m_playerDbId);
  for (const auto shipComputer : shipComputers)
  {
    ids.erase(shipComputer);
  }

  std::vector<bsgo::PlayerComputer> out;
  for (const auto &id : ids)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(id);
    out.push_back(computer);
  }
  return out;
}

auto PlayerView::getPlayerShips() const -> std::vector<bsgo::PlayerShip>
{
  checkPlayerDbIdExists();

  const auto ids = m_repositories.playerShipRepository->findAllByPlayer(*m_playerDbId);

  std::vector<bsgo::PlayerShip> out;
  for (const auto &id : ids)
  {
    const auto ship = m_repositories.playerShipRepository->findOneById(id);
    out.push_back(ship);
  }

  return out;
}

void PlayerView::trySelectShip(const bsgo::Uuid shipDbId) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::HangarMessage>(shipDbId));
}

void PlayerView::tryPurchase(const bsgo::Item &type, const bsgo::Uuid itemDbId) const
{
  checkPlayerDbIdExists();
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::PurchaseMessage>(*m_playerDbId, type, itemDbId));
}

void PlayerView::tryLogin(const std::string &name, const std::string &password) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::LoginMessage>(name, password));
}

void PlayerView::tryLogout() const
{
  checkPlayerDbIdExists();
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::LogoutMessage>(*m_playerDbId));
}

void PlayerView::trySignup(const std::string &name,
                           const std::string &password,
                           const bsgo::Faction &faction) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::SignupMessage>(name, password, faction));
}

void PlayerView::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

} // namespace pge
