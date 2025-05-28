
#include "EffectComponent.hh"

namespace bsgo {

EffectComponent::EffectComponent(const ComponentType &type, const core::Duration &duration)
  : AbstractComponent(type)
  , m_duration(duration)
{
  addModule("effect");
}

bool EffectComponent::isFinished() const
{
  return m_duration < m_elapsedSinceStart;
}

auto EffectComponent::damageModifier() const -> std::optional<float>
{
  return {};
}

void EffectComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  m_elapsedSinceStart += core::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));
}

} // namespace bsgo
