
#include "SystemView.hh"
#include "CircleBox.hh"

namespace bsgo {

SystemView::SystemView()
  : utils::CoreObject("system")
{
  setService("view");
  init();
}

auto SystemView::getAsteroidPositions() const -> std::vector<Eigen::Vector3f>
{
  std::vector<Eigen::Vector3f> out;

  for (const auto &asteroid : m_asteroids)
  {
    const auto ent = m_coordinator.getEntity(asteroid);
    out.push_back(ent.transform->position());
  }

  return out;
}

auto SystemView::getEntity(const Uuid &ent) const -> Entity
{
  return m_coordinator.getEntity(ent);
}

auto SystemView::getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Entity>
{
  const auto ent = m_coordinator.getTransformSystem().getEntityAt(pos);
  if (!ent)
  {
    return {};
  }

  return {m_coordinator.getEntity(*ent)};
}

auto SystemView::getEntitiesWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto uuids = m_coordinator.getTransformSystem().getEntitiesWithin(bbox);

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator.getEntity(uuid));
  }

  return out;
}

void SystemView::init()
{
  constexpr auto SHIP_RADIUS = 1.0f;
  Eigen::Vector3f pos        = Eigen::Vector3f::Zero();
  auto box                   = std::make_unique<CircleBox>(pos, SHIP_RADIUS);
  const auto ship            = m_coordinator.createEntityWithTransform(std::move(box));
  m_ships.insert(ship);

  constexpr auto ASTEROID_RADIUS = 1.0f;
  pos                            = Eigen::Vector3f(1.0f, 2.0f, 0.0f);
  box                            = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  auto asteroid                  = m_coordinator.createEntityWithTransform(std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(4.0f, 2.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(-2.0f, -6.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(std::move(box));
  m_asteroids.insert(asteroid);

  pos      = Eigen::Vector3f(-3.0f, -4.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  asteroid = m_coordinator.createEntityWithTransform(std::move(box));
  m_asteroids.insert(asteroid);
}

} // namespace bsgo
