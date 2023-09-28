
#pragma once

#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShipView : public utils::CoreObject
{
  public:
  ShipView();

  auto getUuid() const noexcept -> Uuid;

  auto getHealth() const noexcept -> float;
  auto getMaxHealth() const noexcept -> float;
  auto getPower() const noexcept -> float;
  auto getMaxPower() const noexcept -> float;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
