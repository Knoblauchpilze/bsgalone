
#include "SystemView.hh"

namespace bsgo {

SystemView::SystemView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : AbstractView("system", coordinator, repositories)
{}

auto SystemView::getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Entity>
{
  const auto id = m_coordinator->getEntityAt(pos, {});
  if (!id)
  {
    return {};
  }
  return m_coordinator->getEntity(*id);
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

auto SystemView::getAsteroid(const Uuid &asteroid) const -> Entity
{
  return m_coordinator->getEntity(asteroid);
}

} // namespace bsgo
