
#pragma once

#include "EffectComponent.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

class WeaponEffectComponent : public EffectComponent
{
  public:
  WeaponEffectComponent(const utils::Duration &duration, const float damageModifier);
  ~WeaponEffectComponent() override = default;

  auto damageModifier() const -> std::optional<float> override;

  private:
  float m_damageModifier{1.0f};
};

} // namespace bsgo
