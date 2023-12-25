
#pragma once

#include "AbstractComponent.hh"
#include "Item.hh"
#include "Uuid.hh"
#include <unordered_set>

namespace bsgo {

class LootComponent : public AbstractComponent
{
  public:
  LootComponent();
  ~LootComponent() override = default;

  void registerRecipient(const Uuid &entity);

  auto recipients() const -> std::unordered_set<Uuid>;

  void update(const float elapsedSeconds) override;

  private:
  std::unordered_set<Uuid> m_recipients{};
};

using LootComponentShPtr = std::shared_ptr<LootComponent>;

} // namespace bsgo
