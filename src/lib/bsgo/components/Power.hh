
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgo {

class Power : public RegenerativeComponent
{
  public:
  // Power();
  Power(const float current, const float max, const float regen);
  ~Power() = default;
};

using PowerShPtr = std::shared_ptr<Power>;

} // namespace bsgo
