
#include "SystemView.hh"

namespace bsgo {

SystemView::SystemView(CoordinatorShPtr coordinator, const DatabaseEntityMapper &entityMapper)
  : AbstractView("system")
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

auto SystemView::getAsteroidsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::ASTEROID});

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getOutpostsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::OUTPOST});

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getBulletsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::BULLET});

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto SystemView::getAsteroid(const Uuid asteroidDbId) const -> Entity
{
  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    error("Failed to get asteroid " + str(asteroidDbId));
  }

  return m_coordinator->getEntity(*maybeAsteroid);
}

} // namespace bsgo
