
#pragma once

#include "EffectComponent.hh"

namespace bsgalone::core {

class WeaponEffectComponent : public EffectComponent
{
  public:
  WeaponEffectComponent(const chrono::TickDuration &duration, const float damageModifier);
  ~WeaponEffectComponent() override = default;

  auto damageModifier() const -> std::optional<float> override;

  private:
  float m_damageModifier{1.0f};
};

} // namespace bsgalone::core
