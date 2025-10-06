
#pragma once

#include "EffectComponent.hh"
#include "TickDuration.hh"

namespace bsgo {

class WeaponEffectComponent : public EffectComponent
{
  public:
  WeaponEffectComponent(const TickDuration &duration, const float damageModifier);
  ~WeaponEffectComponent() override = default;

  auto damageModifier() const -> std::optional<float> override;

  private:
  float m_damageModifier{1.0f};
};

} // namespace bsgo
