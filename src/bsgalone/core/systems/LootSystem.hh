
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class LootSystem : public AbstractSystem
{
  public:
  LootSystem();
  ~LootSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  private:
  void distributeLoot(const Entity &entity, Coordinator &coordinator) const;
  void distributeLootTo(const Uuid recipient,
                        const Entity &deadTarget,
                        Coordinator &coordinator) const;
  void distributeResourcesTo(const Entity &player, const Entity &deadTarget) const;
};

} // namespace bsgo
