
#include "SystemView.hh"
#include "CircleBox.hh"

namespace bsgo {

SystemView::SystemView()
  : utils::CoreObject("system")
{
  setService("view");
  init();
}

auto SystemView::getShipPosition(const Uuid &uuid) const -> Eigen::Vector3f
{
  return m_coordinator.getTransformComponent(uuid)->position();
}

auto SystemView::getAsteroidPositions() const -> std::vector<Eigen::Vector3f>
{
  std::vector<Eigen::Vector3f> out;

  for (const auto &asteroid : m_asteroids)
  {
    const auto p = m_coordinator.getTransformComponent(asteroid)->position();
    out.push_back(p);
  }

  return out;
}

void SystemView::init()
{
  constexpr auto SHIP_RADIUS = 1.0f;
  const auto ship            = m_coordinator.createEntity();
  Eigen::Vector3f pos        = Eigen::Vector3f::Zero();
  auto box                   = std::make_unique<CircleBox>(pos, SHIP_RADIUS);
  m_coordinator.addTransformComponent(ship, std::move(box));
  m_ships.insert(ship);

  constexpr auto ASTEROID_RADIUS = 1.0f;
  auto asteroid                  = m_coordinator.createEntity();
  pos                            = Eigen::Vector3f(1.0f, 2.0f, 0.0f);
  box                            = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  m_coordinator.addTransformComponent(asteroid, std::move(box));
  m_asteroids.insert(asteroid);

  asteroid = m_coordinator.createEntity();
  pos      = Eigen::Vector3f(4.0f, 2.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  m_coordinator.addTransformComponent(asteroid, std::move(box));
  m_asteroids.insert(asteroid);

  asteroid = m_coordinator.createEntity();
  pos      = Eigen::Vector3f(-2.0f, -6.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  m_coordinator.addTransformComponent(asteroid, std::move(box));
  m_asteroids.insert(asteroid);

  asteroid = m_coordinator.createEntity();
  pos      = Eigen::Vector3f(-3.0f, -4.0f, 0.0f);
  box      = std::make_unique<CircleBox>(pos, ASTEROID_RADIUS);
  m_coordinator.addTransformComponent(asteroid, std::move(box));
  m_asteroids.insert(asteroid);
}

} // namespace bsgo
