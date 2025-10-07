
#include "EffectComponent.hh"

namespace bsgo {

EffectComponent::EffectComponent(const ComponentType &type, const TickDuration &duration)
  : AbstractComponent(type)
  , m_duration(duration)
{
  addModule("effect");
}

bool EffectComponent::isFinished() const
{
  // TODO: We should not convert to milliseconds here.
  constexpr auto MILLI_IN_ONE_SECOND = 1000.0f;
  const auto durationAsTime = core::toMilliseconds(MILLI_IN_ONE_SECOND * m_duration.toSeconds());
  return durationAsTime < m_elapsedSinceStart;
}

auto EffectComponent::damageModifier() const -> std::optional<float>
{
  return {};
}

void EffectComponent::update(const TickData &data)
{
  // TODO: We should not convert to real time.
  constexpr auto MILLI_IN_ONE_SECOND = 1000.0f;
  const auto elapsed = core::toMilliseconds(MILLI_IN_ONE_SECOND * data.elapsed.toSeconds());
  m_elapsedSinceStart += elapsed;
}

} // namespace bsgo
