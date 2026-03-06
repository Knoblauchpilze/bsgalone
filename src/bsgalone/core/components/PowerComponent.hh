
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgalone::core {

class PowerComponent : public RegenerativeComponent
{
  public:
  PowerComponent(const float current, const float max, const float regen);
  ~PowerComponent() override = default;

  void usePower(const float power);
  void useAll();
};

using PowerComponentShPtr = std::shared_ptr<PowerComponent>;

} // namespace bsgalone::core
