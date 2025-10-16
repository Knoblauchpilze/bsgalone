
#include "WeaponEffectComponent.hh"

namespace bsgo {

WeaponEffectComponent::WeaponEffectComponent(const chrono::TickDuration &duration,
                                             const float damageModifier)
  : EffectComponent(ComponentType::WEAPON_EFFECT, duration)
  , m_damageModifier(damageModifier)
{
  addModule("effect");
}

auto WeaponEffectComponent::damageModifier() const -> std::optional<float>
{
  return {m_damageModifier};
}

} // namespace bsgo
