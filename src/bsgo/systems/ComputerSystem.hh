
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class ComputerSystem : public AbstractSystem
{
  public:
  ComputerSystem();
  ~ComputerSystem() override = default;

  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  void updateComputer(const Entity &ent,
                      const ComputerSlotComponentShPtr &computer,
                      const std::optional<Entity> &target,
                      const TickData &data) const;

  bool processFireRequest(Entity &ent,
                          const ComputerSlotComponentShPtr &computer,
                          std::optional<Entity> &target,
                          Coordinator &coordinator) const;
  void applyEmitterEffects(Entity &ent,
                           const ComputerSlotComponentShPtr &computer,
                           Coordinator &coordinator) const;
  void applyReceiverEffects(Entity &ent,
                            Entity &target,
                            const ComputerSlotComponentShPtr &computer,
                            Coordinator &coordinator) const;

  void sendComponentUpdatedMessage(const Entity &entity,
                                   const SlotComponent &component,
                                   Coordinator &coordinator) const;
};

} // namespace bsgo
