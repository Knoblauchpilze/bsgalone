
#pragma once

#include "ISystem.hh"

namespace bsgo {

class SlotSystem : public ISystem
{
  public:
  SlotSystem();
  ~SlotSystem() override = default;

  void update(const Components &components, const float elapsedSeconds) override;
};

} // namespace bsgo
