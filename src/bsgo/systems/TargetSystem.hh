
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class TargetSystem : public AbstractSystem
{
  public:
  TargetSystem();
  ~TargetSystem() override = default;

  protected:
  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  void clearTargetIfNotReachable(TargetComponent &targetComp, const Coordinator &coordinator) const;
};

} // namespace bsgo
