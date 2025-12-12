
#include "SystemView.hh"
#include "PlayerListMessage.hh"

namespace pge {

SystemView::SystemView(bsgo::CoordinatorShPtr coordinator,
                       const bsgo::DatabaseEntityMapper &entityMapper)
  : AbstractView("system", {bsgo::MessageType::PLAYER_LIST})
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
  return !m_players.empty();
}

void SystemView::reset()
{
  m_players.clear();
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

void SystemView::handleMessageInternal(const bsgo::IMessage &message)
{
  const auto &playerMessage = message.as<bsgo::PlayerListMessage>();
  m_players                 = playerMessage.getPlayersData();

  debug("Received " + std::to_string(m_players.size()) + " player(s) for system "
        + bsgo::str(playerMessage.getSystemDbId()));
}

} // namespace pge
