
#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class DamageComponent : public AbstractComponent
{
  public:
  DamageComponent(const float damage);
  ~DamageComponent() override = default;

  auto damage() const noexcept -> float;

  void update(const float elapsedSeconds) override;

  private:
  float m_damage;
};

using DamageComponentShPtr = std::shared_ptr<DamageComponent>;

} // namespace bsgo
