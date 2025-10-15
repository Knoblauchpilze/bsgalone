
#include "EffectComponent.hh"

namespace bsgo {

EffectComponent::EffectComponent(const ComponentType &type, const chrono::TickDuration &duration)
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

void EffectComponent::update(const chrono::TickData &data)
{
  m_elapsedSinceStart += data.elapsed;
}

} // namespace bsgo
