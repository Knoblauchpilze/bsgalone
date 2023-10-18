
#include "TargetView.hh"

namespace bsgo {

TargetView::TargetView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator)
  : IView("target")
  , m_playerShipId(playerShipId)
  , m_coordinator(coordinator)
{}

void TargetView::setTarget(const Uuid &uuid)
{
  m_target = uuid;
}

auto TargetView::getTarget() const -> std::optional<Entity>
{
  if (!m_target)
  {
    return {};
  }
  return m_coordinator->getEntity(*m_target);
}

auto TargetView::distanceToTarget() const -> float
{
  if (!m_target)
  {
    error("Can't get distance to non existing target");
  }

  const auto playerPos = getPlayerShipPosition();
  const auto target    = m_coordinator->getEntity(*m_target);
  const auto targetPos = target.access<bsgo::TransformComponent>().position();

  return (targetPos - playerPos).norm();
}

void TargetView::clearTarget()
{
  m_target.reset();
}

auto TargetView::getPlayerShipPosition() const -> Eigen::Vector3f
{
  const auto ship = m_coordinator->getEntity(m_playerShipId);
  return ship.access<TransformComponent>().position();
}

} // namespace bsgo
