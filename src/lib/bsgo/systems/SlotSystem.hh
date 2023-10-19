
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class SlotSystem : public AbstractSystem
{
  public:
  SlotSystem();
  ~SlotSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;
};

} // namespace bsgo
