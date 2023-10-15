
#pragma once

#include "ISystem.hh"

namespace bsgo {

class PowerSystem : public ISystem
{
  public:
  PowerSystem();
  ~PowerSystem() override = default;

  void update(const Components &components, const float elapsedSeconds) override;
};

} // namespace bsgo
