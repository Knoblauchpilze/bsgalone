
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class ComputerSystem : public AbstractSystem
{
  public:
  ComputerSystem();
  ~ComputerSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  void updateComputer(const Entity &ent,
                      const ComputerSlotComponentShPtr &computer,
                      const std::optional<Entity> &target,
                      const float elapsedSeconds) const;

  void processFireRequest(Entity &ent,
                          const ComputerSlotComponentShPtr &computer,
                          std::optional<Entity> &target,
                          Coordinator &coordinator) const;
  void applyEmitterEffects(Entity &ent,
                           const ComputerSlotComponentShPtr &computer,
                           Coordinator &coordinator) const;
  void applyReceiverEffects(Entity &target,
                            const ComputerSlotComponentShPtr &computer,
                            Coordinator &coordinator) const;
};

} // namespace bsgo
