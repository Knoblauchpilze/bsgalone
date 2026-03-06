
#include "SystemView.hh"
#include "JoinShipMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerShipListMessage.hh"

namespace bsgalone::client {

SystemView::SystemView(core::CoordinatorShPtr coordinator,
                       const core::DatabaseEntityMapper &entityMapper)
  : AbstractView("system",
                 {core::MessageType::PLAYER_LIST,
                  core::MessageType::PLAYER_SHIP_LIST,
                  core::MessageType::JOIN_SHIP})
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

bool SystemView::isReady() const noexcept
{
  // TODO: Here an in other places, this check is not suited for cases where
  // there are really no players in a system.
  return !m_players.empty() && !m_playerShips.empty();
}

void SystemView::reset()
{
  m_players.clear();
  m_playerShips.clear();
}

auto SystemView::getAsteroidsWithin(const core::IBoundingBox &bbox) const
  -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::ASTEROID});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getOutpostsWithin(const core::IBoundingBox &bbox) const
  -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::OUTPOST});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getBulletsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::BULLET});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getAsteroid(const core::Uuid asteroidDbId) const -> core::Entity
{
  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    error("Failed to get asteroid " + core::str(asteroidDbId));
  }

  return m_coordinator->getEntity(*maybeAsteroid);
}

auto SystemView::getPlayer(const core::Uuid playerDbId) const -> core::PlayerData
{
  const auto maybePlayer = std::find_if(m_players.begin(),
                                        m_players.end(),
                                        [playerDbId](const core::PlayerData &player) {
                                          return player.dbId == playerDbId;
                                        });

  if (maybePlayer == m_players.end())
  {
    error("Failed to get player " + core::str(playerDbId));
  }

  return *maybePlayer;
}

auto SystemView::getSystemPlayers() const -> std::vector<core::PlayerData>
{
  return m_players;
}

auto SystemView::getSystemShips() const -> std::vector<core::PlayerShipData>
{
  return m_playerShips;
}

namespace {
auto getPlayersList(const core::PlayerListMessage &message) -> std::vector<core::PlayerData>
{
  return message.getPlayersData();
}

auto getPlayerShipsList(const core::PlayerShipListMessage &message)
  -> std::vector<core::PlayerShipData>
{
  // Ignore the message in case the message has a player identifier. In this case
  // the message does not contain the list of ships in a system but the list of
  // ships owned by a specific player. This is not what is needed by this view.
  if (message.tryGetPlayerDbId())
  {
    return {};
  }

  return message.getShipsData();
}

void tryUpdatePlayer(const core::JoinShipMessage &updatedData,
                     std::vector<core::PlayerData> &players)
{
  const auto playerDbId = updatedData.getPlayerDbId();

  const auto maybePlayer = std::find_if(players.begin(),
                                        players.end(),
                                        [playerDbId](const core::PlayerData &playerData) {
                                          return playerData.dbId == playerDbId;
                                        });

  if (maybePlayer == players.end())
  {
    return;
  }

  maybePlayer->attachedShip = updatedData.getShipDbId();
}
} // namespace

void SystemView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::JOIN_SHIP:
      tryUpdatePlayer(message.as<core::JoinShipMessage>(), m_players);
      break;
    case core::MessageType::PLAYER_LIST:
      m_players = getPlayersList(message.as<core::PlayerListMessage>());
      break;
    case core::MessageType::PLAYER_SHIP_LIST:
      m_playerShips = getPlayerShipsList(message.as<core::PlayerShipListMessage>());
      break;
    default:
      error("Unsupported message type received in system view", "Received " + str(message.type()));
  }

  debug("Received " + str(message.type()) + " message");
}

} // namespace bsgalone::client
