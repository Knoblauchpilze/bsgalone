
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgo {

class Health : public RegenerativeComponent
{
  public:
  Health(const float current, const float max, const float regen);
  ~Health() override = default;
};

using HealthShPtr = std::shared_ptr<Health>;

} // namespace bsgo
