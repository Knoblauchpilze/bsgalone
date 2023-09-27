
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView()
  : utils::CoreObject("ship")
{
  setService("view");
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

} // namespace bsgo
