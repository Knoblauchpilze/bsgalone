
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgo {

class PowerComponent : public RegenerativeComponent
{
  public:
  PowerComponent(const float current, const float max, const float regen);
  ~PowerComponent() override = default;
};

using PowerComponentShPtr = std::shared_ptr<PowerComponent>;

} // namespace bsgo
