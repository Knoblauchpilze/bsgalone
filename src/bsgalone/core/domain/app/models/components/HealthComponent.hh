
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgalone::core {

struct HealthComponent : public RegenerativeComponent
{
  bool isAlive() const;
};

} // namespace bsgalone::core
