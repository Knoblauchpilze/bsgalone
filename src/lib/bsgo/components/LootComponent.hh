
#pragma once

#include "IComponent.hh"
#include "Item.hh"
#include "Uuid.hh"
#include <unordered_set>

namespace bsgo {

class LootComponent : public IComponent
{
  public:
  LootComponent(const Uuid &loot, const Item &type);
  ~LootComponent() override = default;

  auto loot() const -> Uuid;
  auto type() const -> Item;
  void registerRecipient(const Uuid &entity);

  auto recipients() const -> std::unordered_set<Uuid>;

  void update(const float elapsedSeconds) override;

  private:
  Uuid m_loot;
  Item m_type;
  std::unordered_set<Uuid> m_recipients{};
};

using LootComponentShPtr = std::shared_ptr<LootComponent>;

} // namespace bsgo