
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class LootSystem : public AbstractSystem
{
  public:
  LootSystem();
  ~LootSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  void distributeLoot(const LootComponent &loot, Coordinator &coordinator) const;
  void distributeLootTo(const Uuid &recipient,
                        const LootComponent &loot,
                        Coordinator &coordinator) const;
  void distributeResourceTo(const Uuid &player, const Uuid &loot, Coordinator &coordinator) const;
};

} // namespace bsgo
