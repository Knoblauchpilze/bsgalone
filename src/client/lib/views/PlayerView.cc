
#include "PlayerView.hh"
#include "HangarMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PurchaseMessage.hh"
#include "SignupMessage.hh"

namespace pge {

PlayerView::PlayerView(const bsgo::Repositories &repositories,
                       GameSessionShPtr gameSession,
                       bsgo::IMessageQueue *const outputMessageQueue)
  : AbstractView("player")
  , m_repositories(repositories)
  , m_gameSession(std::move(gameSession))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_gameSession)
  {
    throw std::invalid_argument("Expected non null game session");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

bool PlayerView::isReady() const noexcept
{
  return m_gameSession->hasPlayerDbId();
}

auto PlayerView::getPlayerDbId() const -> bsgo::Uuid
{
  checkPlayerDbIdExists();
  return m_gameSession->getPlayerDbId();
}

auto PlayerView::getPlayerFaction() const -> bsgo::Faction
{
  checkPlayerDbIdExists();
  return m_gameSession->getFaction();
}

auto PlayerView::getPlayerResources() const -> std::vector<bsgo::PlayerResource>
{
  checkPlayerDbIdExists();
  return m_repositories.playerResourceRepository->findAllByPlayer(m_gameSession->getPlayerDbId());
}

auto PlayerView::getPlayerWeapons() const -> std::vector<bsgo::PlayerWeapon>
{
  checkPlayerDbIdExists();

  std::vector<bsgo::ShipWeapon> shipWeapons{};
  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(
    m_gameSession->getPlayerDbId());
  for (const auto &shipId : ships)
  {
    const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(shipId);
    shipWeapons.insert(shipWeapons.end(), weapons.begin(), weapons.end());
  }

  auto ids = m_repositories.playerWeaponRepository->findAllByPlayer(m_gameSession->getPlayerDbId());
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
  const auto ships = m_repositories.playerShipRepository->findAllByPlayer(
    m_gameSession->getPlayerDbId());
  for (const auto &shipId : ships)
  {
    const auto computers = m_repositories.shipComputerRepository->findAllByShip(shipId);
    shipComputers.insert(shipComputers.end(), computers.begin(), computers.end());
  }

  auto ids = m_repositories.playerComputerRepository->findAllByPlayer(
    m_gameSession->getPlayerDbId());
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

  const auto ids = m_repositories.playerShipRepository->findAllByPlayer(
    m_gameSession->getPlayerDbId());

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
    std::make_unique<bsgo::PurchaseMessage>(m_gameSession->getPlayerDbId(), type, itemDbId));
}

void PlayerView::tryLogin(const std::string &name, const std::string &password) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::LoginMessage>(name, password));
}

void PlayerView::tryLogout() const
{
  checkPlayerDbIdExists();
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::LogoutMessage>(m_gameSession->getPlayerDbId()));
}

void PlayerView::trySignup(const std::string &name,
                           const std::string &password,
                           const bsgo::Faction &faction) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::SignupMessage>(name, password, faction));
}

void PlayerView::checkPlayerDbIdExists() const
{
  if (!m_gameSession->hasPlayerDbId())
  {
    error("Expected player db id to exist but it does not");
  }
}

} // namespace pge
