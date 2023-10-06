
#pragma once

#include "Components.hh"

namespace bsgo {

class ISystem
{
  public:
  virtual ~ISystem() = default;

  virtual void update(const Components &components, const float elapsedSeconds) = 0;
};

} // namespace bsgo
