
#include "PlayerView.hh"
#include "HangarMessage.hh"
#include "JoinShipMessage.hh"
#include "LoginRequest.hh"
#include "LogoutMessage.hh"
#include "PurchaseMessage.hh"
#include "ShipSelectedMessage.hh"
#include "SignupMessage.hh"

namespace bsgalone::client {

PlayerView::PlayerView(GameSessionShPtr gameSession, core::IMessageQueue *const outputMessageQueue)
  : AbstractView("player",
                 {
                   core::MessageType::HANGAR,
                   core::MessageType::PLAYER_COMPUTER_LIST,
                   core::MessageType::PLAYER_RESOURCE_LIST,
                   core::MessageType::PLAYER_SHIP_LIST,
                   core::MessageType::PLAYER_WEAPON_LIST,
                 })
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
  // Computers and weapons are allowed to be empty in case the player
  // sells everything.
  return m_gameSession->hasPlayerDbId() && m_gameSession->hasTimeStep()
         && !m_playerResources.empty() && !m_playerShips.empty();
}

void PlayerView::reset()
{
  m_playerResources.clear();
  m_playerShips.clear();
  m_playerComputers.clear();
  m_playerWeapons.clear();
}

auto PlayerView::gameSession() const -> const GameSession &
{
  return *m_gameSession;
}

auto PlayerView::getPlayerFaction() const -> core::Faction
{
  return m_gameSession->getFaction();
}

auto PlayerView::getPlayerResources() const -> std::vector<core::PlayerResourceData>
{
  return m_playerResources;
}

namespace {
auto getAllWeaponsOnShips(const std::vector<core::PlayerShipData> &playerShips)
  -> std::unordered_set<core::Uuid>
{
  std::unordered_set<core::Uuid> weaponsOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<core::Uuid> weaponIds;
    std::transform(ship.weapons.begin(),
                   ship.weapons.end(),
                   std::back_inserter(weaponIds),
                   [](const core::PlayerWeaponData &weapon) { return weapon.dbId; });

    weaponsOnShips.insert(weaponIds.begin(), weaponIds.end());
  }

  return weaponsOnShips;
}
} // namespace

auto PlayerView::getPlayerWeapons() const -> std::vector<core::PlayerWeaponData>
{
  const auto weaponsInstalledOnShips = getAllWeaponsOnShips(m_playerShips);

  std::vector<core::PlayerWeaponData> out(m_playerWeapons);

  std::erase_if(out, [&weaponsInstalledOnShips](const core::PlayerWeaponData &weaponData) {
    return weaponsInstalledOnShips.contains(weaponData.dbId);
  });

  return out;
}

namespace {
auto getAllComputersOnShips(const std::vector<core::PlayerShipData> &playerShips)
  -> std::unordered_set<core::Uuid>
{
  std::unordered_set<core::Uuid> computersOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<core::Uuid> computerIds;
    std::transform(ship.computers.begin(),
                   ship.computers.end(),
                   std::back_inserter(computerIds),
                   [](const core::PlayerComputerData &computer) { return computer.dbId; });

    computersOnShips.insert(computerIds.begin(), computerIds.end());
  }

  return computersOnShips;
}
} // namespace

auto PlayerView::getPlayerComputers() const -> std::vector<core::PlayerComputerData>
{
  const auto computersInstalledOnShips = getAllComputersOnShips(m_playerShips);

  std::vector<core::PlayerComputerData> out(m_playerComputers);

  std::erase_if(out, [&computersInstalledOnShips](const core::PlayerComputerData &computerData) {
    return computersInstalledOnShips.contains(computerData.dbId);
  });

  return out;
}

auto PlayerView::getPlayerShips() const -> std::vector<core::PlayerShipData>
{
  return m_playerShips;
}

void PlayerView::trySelectShip(const core::Uuid shipDbId) const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::ShipSelectedMessage>(m_gameSession->getPlayerDbId(),
                                                m_gameSession->getSystemDbId(),
                                                shipDbId));
}

void PlayerView::tryPurchase(const core::Item &type, const core::Uuid itemDbId) const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::PurchaseMessage>(m_gameSession->getPlayerDbId(),
                                            m_gameSession->getSystemDbId(),
                                            type,
                                            itemDbId));
}

void PlayerView::tryLogin(const std::string &name,
                          const std::string &password,
                          const core::GameRole role) const
{
  auto out = std::make_unique<core::LoginRequest>(name, password, role);
  m_outputMessageQueue->pushEvent(std::move(out));
}

void PlayerView::tryLogout() const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::LogoutMessage>(m_gameSession->getPlayerDbId()));
}

void PlayerView::trySignup(const std::string &name,
                           const std::string &password,
                           const core::Faction &faction) const
{
  m_outputMessageQueue->pushEvent(std::make_unique<core::SignupMessage>(name, password, faction));
}

void PlayerView::tryJoin(const core::Uuid playerDbId, const core::Uuid shipDbId) const
{
  m_outputMessageQueue->pushEvent(std::make_unique<core::JoinShipMessage>(playerDbId, shipDbId));
}

void PlayerView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::HANGAR:
      handleHangarMessage(message.as<core::HangarMessage>());
      break;
    case core::MessageType::PLAYER_COMPUTER_LIST:
      handlePlayerComputersMessage(message.as<core::PlayerComputerListMessage>());
      break;
    case core::MessageType::PLAYER_RESOURCE_LIST:
      handlePlayerResourcesMessage(message.as<core::PlayerResourceListMessage>());
      break;
    case core::MessageType::PLAYER_SHIP_LIST:
      handlePlayerShipsMessage(message.as<core::PlayerShipListMessage>());
      break;
    case core::MessageType::PLAYER_WEAPON_LIST:
      handlePlayerWeaponsMessage(message.as<core::PlayerWeaponListMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      break;
  }
}

void PlayerView::handleHangarMessage(const core::HangarMessage &message)
{
  // Replace the active ship with the one from the message in the list.
  const auto maybeActiveShip = std::find_if(m_playerShips.begin(),
                                            m_playerShips.end(),
                                            [&message](const core::PlayerShipData &ship) {
                                              return ship.dbId == message.getShipDbId();
                                            });
  if (maybeActiveShip == m_playerShips.end())
  {
    error("Received hangar message for unknown ship " + core::str(message.getShipDbId()));
    return;
  }

  *maybeActiveShip = message.getShip();
}

void PlayerView::handlePlayerComputersMessage(const core::PlayerComputerListMessage &message)
{
  m_playerComputers = message.getComputersData();
}

void PlayerView::handlePlayerResourcesMessage(const core::PlayerResourceListMessage &message)
{
  m_playerResources = message.getResourcesData();
}

namespace {
bool doesMessageContainsPlayerShips(const core::PlayerShipListMessage &message)
{
  return message.tryGetPlayerDbId().has_value();
}
} // namespace

void PlayerView::handlePlayerShipsMessage(const core::PlayerShipListMessage &message)
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
    error("Received ships message for wrong player " + core::str(playerDbId),
          "Expected message for player " + core::str(m_gameSession->getPlayerDbId()));
  }

  m_playerShips = message.getShipsData();
}

void PlayerView::handlePlayerWeaponsMessage(const core::PlayerWeaponListMessage &message)
{
  m_playerWeapons = message.getWeaponsData();
}

} // namespace bsgalone::client
