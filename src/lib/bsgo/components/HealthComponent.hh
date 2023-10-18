
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgo {

class HealthComponent : public RegenerativeComponent
{
  public:
  HealthComponent(const float current, const float max, const float regen);
  ~HealthComponent() override = default;
};

using HealthComponentShPtr = std::shared_ptr<HealthComponent>;

} // namespace bsgo
