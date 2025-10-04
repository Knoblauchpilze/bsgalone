
#pragma once

#include "EffectComponent.hh"
#include "Tick.hh"

namespace bsgo {

class WeaponEffectComponent : public EffectComponent
{
  public:
  WeaponEffectComponent(const Tick &duration, const float damageModifier);
  ~WeaponEffectComponent() override = default;

  auto damageModifier() const -> std::optional<float> override;

  private:
  float m_damageModifier{1.0f};
};

} // namespace bsgo
