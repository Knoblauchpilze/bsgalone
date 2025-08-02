
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
  return m_gameSession->hasPlayerDbId() && !m_playerResources.empty();
}

void PlayerView::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::PLAYER_COMPUTER_LIST:
      handlePlayerComputersMessage(message.as<bsgo::PlayerComputerListMessage>());
      break;
    case bsgo::MessageType::PLAYER_RESOURCE_LIST:
      handlePlayerResourcesMessage(message.as<bsgo::PlayerResourceListMessage>());
      break;
    case bsgo::MessageType::PLAYER_SHIP_LIST:
      handlePlayerShipsMessage(message.as<bsgo::PlayerShipListMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
      break;
  }
}

auto PlayerView::getPlayerDbId() const -> bsgo::Uuid
{
  return m_gameSession->getPlayerDbId();
}

auto PlayerView::getPlayerFaction() const -> bsgo::Faction
{
  return m_gameSession->getFaction();
}

auto PlayerView::getPlayerResources() const -> std::vector<bsgo::PlayerResourceData>
{
  return m_playerResources;
}

auto PlayerView::getPlayerWeapons() const -> std::vector<bsgo::PlayerWeapon>
{
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

auto PlayerView::getPlayerComputers() const -> std::vector<bsgo::PlayerComputerData>
{
  const auto computerInstalledOnShips = m_repositories.playerComputerRepository->findAllByPlayer(
    m_gameSession->getPlayerDbId());

  std::vector<bsgo::PlayerComputerData> out(m_playerComputers);
  std::erase_if(out, [&computerInstalledOnShips](const bsgo::PlayerComputerData &computerData) {
    return computerInstalledOnShips.contains(computerData.dbId);
  });

  return out;
}

auto PlayerView::getPlayerShips() const -> std::vector<bsgo::PlayerShipData>
{
  return m_playerShips;
}

void PlayerView::trySelectShip(const bsgo::Uuid shipDbId) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::HangarMessage>(shipDbId));
}

void PlayerView::tryPurchase(const bsgo::Item &type, const bsgo::Uuid itemDbId) const
{
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::PurchaseMessage>(m_gameSession->getPlayerDbId(), type, itemDbId));
}

void PlayerView::tryLogin(const std::string &name, const std::string &password) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::LoginMessage>(name, password));
}

void PlayerView::tryLogout() const
{
  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::LogoutMessage>(m_gameSession->getPlayerDbId()));
}

void PlayerView::trySignup(const std::string &name,
                           const std::string &password,
                           const bsgo::Faction &faction) const
{
  m_outputMessageQueue->pushMessage(std::make_unique<bsgo::SignupMessage>(name, password, faction));
}

void PlayerView::handlePlayerComputersMessage(const bsgo::PlayerComputerListMessage &message)
{
  m_playerComputers = message.getComputersData();
}

void PlayerView::handlePlayerResourcesMessage(const bsgo::PlayerResourceListMessage &message)
{
  m_playerResources = message.getResourcesData();
}

void PlayerView::handlePlayerShipsMessage(const bsgo::PlayerShipListMessage &message)
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!m_gameSession->hasPlayerDbId())
  {
    error("Game session has no player identifier yet");
  }
  if (!maybePlayerDbId)
  {
    error("Player identifier for ships message is undefined");
  }
  if (m_gameSession->getPlayerDbId() != *maybePlayerDbId)
  {
    error("Received ships message for wrong player " + bsgo::str(*maybePlayerDbId),
          "Expected message for player " + bsgo::str(m_gameSession->getPlayerDbId()));
  }

  m_playerShips = message.getShipsData();
}

} // namespace pge
