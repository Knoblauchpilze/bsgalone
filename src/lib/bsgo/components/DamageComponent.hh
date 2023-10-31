
#pragma once

#include "IComponent.hh"

namespace bsgo {

class DamageComponent : public IComponent
{
  public:
  DamageComponent(const float damage);
  ~DamageComponent() = default;

  auto damage() const noexcept -> float;

  void update(const float elapsedSeconds) override;

  private:
  float m_damage;
};

using DamageComponentShPtr = std::shared_ptr<DamageComponent>;

} // namespace bsgo
