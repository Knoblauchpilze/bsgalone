
#include "SystemView.hh"
#include "PlayerListMessage.hh"
#include "PlayerShipListMessage.hh"

#include <iostream>

namespace pge {

SystemView::SystemView(bsgo::CoordinatorShPtr coordinator,
                       const bsgo::DatabaseEntityMapper &entityMapper)
  : AbstractView("system", {bsgo::MessageType::PLAYER_LIST, bsgo::MessageType::PLAYER_SHIP_LIST})
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

auto SystemView::getAsteroidsWithin(const bsgo::IBoundingBox &bbox) const
  -> std::vector<bsgo::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {bsgo::EntityKind::ASTEROID});

  std::vector<bsgo::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getOutpostsWithin(const bsgo::IBoundingBox &bbox) const
  -> std::vector<bsgo::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {bsgo::EntityKind::OUTPOST});

  std::vector<bsgo::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getBulletsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {bsgo::EntityKind::BULLET});

  std::vector<bsgo::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getAsteroid(const bsgo::Uuid asteroidDbId) const -> bsgo::Entity
{
  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    error("Failed to get asteroid " + bsgo::str(asteroidDbId));
  }

  return m_coordinator->getEntity(*maybeAsteroid);
}

auto SystemView::getSystemPlayers() const -> std::vector<bsgo::PlayerData>
{
  return m_players;
}

auto SystemView::getSystemShips() const -> std::vector<bsgo::PlayerShipData>
{
  return m_playerShips;
}

namespace {
auto getPlayersList(const bsgo::PlayerListMessage &message) -> std::vector<bsgo::PlayerData>
{
  return message.getPlayersData();
}

auto getPlayerShipsList(const bsgo::PlayerShipListMessage &message)
  -> std::vector<bsgo::PlayerShipData>
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
} // namespace

void SystemView::handleMessageInternal(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::PLAYER_LIST:
      m_players = getPlayersList(message.as<bsgo::PlayerListMessage>());
      break;
    case bsgo::MessageType::PLAYER_SHIP_LIST:
      m_playerShips = getPlayerShipsList(message.as<bsgo::PlayerShipListMessage>());
      break;
    default:
      error("Unsupported message type received in system view", "Received " + str(message.type()));
  }

  debug("Received " + str(message.type()) + " message");
}

} // namespace pge
