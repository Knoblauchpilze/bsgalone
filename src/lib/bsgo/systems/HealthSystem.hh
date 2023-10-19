
#pragma once

#include "ISystem.hh"

namespace bsgo {

class HealthSystem : public ISystem
{
  public:
  HealthSystem();
  ~HealthSystem() override = default;

  void update(const Components &components,
              const Coordinator &coordinator,
              const float elapsedSeconds) override;
};

} // namespace bsgo
