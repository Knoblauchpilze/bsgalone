
#pragma once

#include "AbstractSystem.hh"

namespace bsgalone::core {

class TargetSystem : public AbstractSystem
{
  public:
  TargetSystem();
  ~TargetSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  private:
  bool clearTargetIfNotReachable(TargetComponent &targetComp, const Coordinator &coordinator) const;
  void publishTargetMessage(const Entity &entity) const;
};

} // namespace bsgalone::core
