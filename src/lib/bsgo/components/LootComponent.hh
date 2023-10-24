
#pragma once

#include "IComponent.hh"

namespace bsgo {

class LootComponent : public IComponent
{
  public:
  LootComponent(const float amount);
  ~LootComponent() override = default;

  auto amount() const -> float;

  void update(const float elapsedSeconds) override;

  private:
  float m_amount;
};

using LootComponentShPtr = std::shared_ptr<LootComponent>;

} // namespace bsgo
