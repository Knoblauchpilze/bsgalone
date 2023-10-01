
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView()
  : utils::CoreObject("ship")
{
  setService("view");
}

auto ShipView::getUuid() const noexcept -> Uuid
{
  return {};
}

auto ShipView::getHealth() const noexcept -> float
{
  return 72.3f;
}

auto ShipView::getMaxHealth() const noexcept -> float
{
  return 550.0f;
}

auto ShipView::getPower() const noexcept -> float
{
  return 36.1f;
}

auto ShipView::getMaxPower() const noexcept -> float
{
  return 150.0f;
}

bool ShipView::hasTarget() const noexcept
{
  return false;
}

auto ShipView::getTargetHealth() const noexcept -> float
{
  return 234.0f;
}

auto ShipView::getTargetMaxHealth() const noexcept -> float
{
  return 7698.7f;
}

auto ShipView::getTargetPower() const noexcept -> float
{
  return 33.8f;
}

auto ShipView::getTargetMaxPower() const noexcept -> float
{
  return 35.1f;
}

} // namespace bsgo
