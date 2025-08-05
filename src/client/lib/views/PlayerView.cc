
#include "PlayerView.hh"
#include "HangarMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PurchaseMessage.hh"
#include "SignupMessage.hh"

namespace pge {

PlayerView::PlayerView(GameSessionShPtr gameSession, bsgo::IMessageQueue *const outputMessageQueue)
  : AbstractView("player")
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
    case bsgo::MessageType::PLAYER_WEAPON_LIST:
      handlePlayerWeaponsMessage(message.as<bsgo::PlayerWeaponListMessage>());
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

namespace {
auto getAllWeaponsOnShips(const std::vector<bsgo::PlayerShipData> &playerShips)
  -> std::unordered_set<bsgo::Uuid>
{
  std::unordered_set<bsgo::Uuid> weaponsOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<bsgo::Uuid> weaponIds;
    std::transform(ship.weapons.begin(),
                   ship.weapons.end(),
                   std::back_inserter(weaponIds),
                   [](const bsgo::PlayerWeaponData &weapon) { return weapon.dbId; });

    weaponsOnShips.insert(weaponIds.begin(), weaponIds.end());
  }

  return weaponsOnShips;
}
} // namespace

auto PlayerView::getPlayerWeapons() const -> std::vector<bsgo::PlayerWeaponData>
{
  const auto weaponsInstalledOnShips = getAllWeaponsOnShips(m_playerShips);

  std::vector<bsgo::PlayerWeaponData> out(m_playerWeapons);

  std::erase_if(out, [&weaponsInstalledOnShips](const bsgo::PlayerWeaponData &weaponData) {
    return weaponsInstalledOnShips.contains(weaponData.dbId);
  });

  return out;
}

namespace {
auto getAllComputersOnShips(const std::vector<bsgo::PlayerShipData> &playerShips)
  -> std::unordered_set<bsgo::Uuid>
{
  std::unordered_set<bsgo::Uuid> computersOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<bsgo::Uuid> computerIds;
    std::transform(ship.computers.begin(),
                   ship.computers.end(),
                   std::back_inserter(computerIds),
                   [](const bsgo::PlayerComputerData &computer) { return computer.dbId; });

    computersOnShips.insert(computerIds.begin(), computerIds.end());
  }

  return computersOnShips;
}
} // namespace

auto PlayerView::getPlayerComputers() const -> std::vector<bsgo::PlayerComputerData>
{
  const auto computersInstalledOnShips = getAllComputersOnShips(m_playerShips);

  std::vector<bsgo::PlayerComputerData> out(m_playerComputers);

  std::erase_if(out, [&computersInstalledOnShips](const bsgo::PlayerComputerData &computerData) {
    return computersInstalledOnShips.contains(computerData.dbId);
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

namespace {
bool doesMessageContainsPlayerShips(const bsgo::PlayerShipListMessage &message)
{
  return message.tryGetPlayerDbId().has_value();
}
} // namespace

void PlayerView::handlePlayerShipsMessage(const bsgo::PlayerShipListMessage &message)
{
  // Only consider messages that define ships for a player. This same message can
  // also be used to communicate the ships of a system when the player leaves the
  // outpost.
  if (!doesMessageContainsPlayerShips(message))
  {
    return;
  }

  const auto playerDbId = message.tryGetPlayerDbId().value();
  if (!m_gameSession->hasPlayerDbId())
  {
    error("Game session has no player identifier yet");
  }
  if (m_gameSession->getPlayerDbId() != playerDbId)
  {
    error("Received ships message for wrong player " + bsgo::str(playerDbId),
          "Expected message for player " + bsgo::str(m_gameSession->getPlayerDbId()));
  }

  m_playerShips = message.getShipsData();
}

void PlayerView::handlePlayerWeaponsMessage(const bsgo::PlayerWeaponListMessage &message)
{
  m_playerWeapons = message.getWeaponsData();
}

} // namespace pge
