
#include "TargetView.hh"

namespace bsgo {

TargetView::TargetView(const CoordinatorShPtr &coordinator)
  : utils::CoreObject("target")
  , m_coordinator(coordinator)
{
  setService("view");
}

void TargetView::update(const float /*elapsedSeconds*/) {}

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

void TargetView::clearTarget()
{
  m_target.reset();
}

auto TargetView::getTargetHealth() const noexcept -> float
{
  return 234.0f;
}

auto TargetView::getTargetMaxHealth() const noexcept -> float
{
  return 7698.7f;
}

auto TargetView::getTargetPower() const noexcept -> float
{
  return 33.8f;
}

auto TargetView::getTargetMaxPower() const noexcept -> float
{
  return 35.1f;
}

} // namespace bsgo
