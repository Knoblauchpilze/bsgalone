
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgo {

class Power : public RegenerativeComponent
{
  public:
  Power(const float current, const float max, const float regen);
  ~Power() override = default;
};

using PowerShPtr = std::shared_ptr<Power>;

} // namespace bsgo
