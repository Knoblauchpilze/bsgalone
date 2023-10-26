
#pragma once

#include "IComponent.hh"
#include "Uuid.hh"
#include <unordered_set>

namespace bsgo {

class LootComponent : public IComponent
{
  public:
  LootComponent(const float amount);
  ~LootComponent() override = default;

  auto amount() const -> float;
  void registerRecipient(const Uuid &entity);

  void update(const float elapsedSeconds) override;

  private:
  float m_amount;
  std::unordered_set<Uuid> m_recipients{};
};

using LootComponentShPtr = std::shared_ptr<LootComponent>;

} // namespace bsgo
