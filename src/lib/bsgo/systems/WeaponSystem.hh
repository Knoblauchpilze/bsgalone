
#pragma once

#include "ISystem.hh"

namespace bsgo {

class WeaponSystem : public ISystem
{
  public:
  WeaponSystem();
  ~WeaponSystem() override = default;

  void update(const Components &components,
              const Coordinator &coordinator,
              const float elapsedSeconds) override;
};

} // namespace bsgo
