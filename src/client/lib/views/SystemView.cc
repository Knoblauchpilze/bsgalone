
#include "SystemView.hh"

namespace pge {

SystemView::SystemView(bsgo::CoordinatorShPtr coordinator,
                       const bsgo::DatabaseEntityMapper &entityMapper)
  : AbstractView("system")
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

void SystemView::reset()
{
  // Voluntarily empty.
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

} // namespace pge
