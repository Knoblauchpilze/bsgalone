
#include "SystemView.hh"

namespace bsgo {

SystemView::SystemView(const CoordinatorShPtr &coordinator)
  : utils::CoreObject("system")
  , m_coordinator(coordinator)
{}

void SystemView::update(const float elapsedSeconds)
{
  m_coordinator->update(elapsedSeconds);
}

auto SystemView::getEntity(const Uuid &ent) const -> Entity
{
  return m_coordinator->getEntity(ent);
}

auto SystemView::getEntityAt(const Eigen::Vector3f &pos,
                             const std::optional<EntityKind> &filter) const -> std::optional<Entity>
{
  const auto id = m_coordinator->getEntityAt(pos, filter);
  if (!id)
  {
    return {};
  }
  return m_coordinator->getEntity(*id);
}

auto SystemView::getEntitiesWithin(const IBoundingBox &bbox,
                                   const std::optional<EntityKind> &filter) const
  -> std::vector<Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, filter);

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

} // namespace bsgo
