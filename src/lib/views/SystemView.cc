
#include "SystemView.hh"
#include "CircleBox.hh"

namespace bsgo {

SystemView::SystemView()
  : utils::CoreObject("system")
{
  setService("view");
  init();
}

auto SystemView::getEntity(const Uuid &ent) const -> Entity
{
  return m_coordinator.getEntity(ent);
}

auto SystemView::getEntityAt(const Eigen::Vector3f &pos,
                             const std::optional<EntityKind> &filter) const -> std::optional<Entity>
{
  const auto ent = m_coordinator.getTransformSystem().getEntityAt(pos);
  if (!ent)
  {
    return {};
  }

  if (filter && !isValidForFilter(*ent, *filter))
  {
    return {};
  }

  return {m_coordinator.getEntity(*ent)};
}

auto SystemView::getEntitiesWithin(const IBoundingBox &bbox,
                                   const std::optional<EntityKind> &filter) const
  -> std::vector<Entity>
{
  const auto uuids = m_coordinator.getTransformSystem().getEntitiesWithin(bbox);

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    if (!filter || isValidForFilter(uuid, *filter))
    {
      out.push_back(m_coordinator.getEntity(uuid));
    }
  }

  return out;
}

void SystemView::init()
{
  constexpr auto SHIP_RADIUS = 0.6f;
  Eigen::Vector3f pos        = Eigen::Vector3f::Zero();
  auto box                   = std::make_unique<CircleBox>(pos, SHIP_RADIUS);
  const auto ship = m_coordinator.createEntityWithTransform(EntityKind::SHIP, std::move(box));
  m_ships.insert(ship);

  constexpr auto ASTEROID_RADIUS = 0.5f;
  pos                            = Eigen::Vector3f(1.0f, 2.0f, 0.0f);
  box                            = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  auto asteroid = m_coordinator.createEntityWithTransform(EntityKind::ASTEROID, std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(4.0f, 2.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(EntityKind::ASTEROID, std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(-2.0f, -6.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(EntityKind::ASTEROID, std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(-3.0f, -4.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(EntityKind::ASTEROID, std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(15.0f, 17.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(EntityKind::ASTEROID, std::move(box));
  m_asteroids.insert(asteroid);
}

bool SystemView::isValidForFilter(const Uuid &ent, const EntityKind &kind) const
{
  switch (kind)
  {
    case EntityKind::SHIP:
      return m_ships.contains(ent);
    case EntityKind::ASTEROID:
      return m_asteroids.contains(ent);
    default:
      warn("Unknown filter " + str(kind));
      break;
  }
  return false;
}

} // namespace bsgo
