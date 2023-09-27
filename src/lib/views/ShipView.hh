
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShipView : public utils::CoreObject
{
  public:
  ShipView();

  auto getHealth() const noexcept -> float;
  auto getMaxHealth() const noexcept -> float;
  auto getPower() const noexcept -> float;
  auto getMaxPower() const noexcept -> float;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
