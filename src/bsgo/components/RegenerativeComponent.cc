
#include "RegenerativeComponent.hh"
#include <algorithm>

namespace bsgo {

RegenerativeComponent::RegenerativeComponent(const ComponentType &type,
                                             const float min,
                                             const float value,
                                             const float max,
                                             const float regenPerSecond)
  : AbstractComponent(type)
  , m_min(min)
  , m_value(value)
  , m_max(max)
  , m_regenPerSecond(regenPerSecond)
{
  validate();
}

void RegenerativeComponent::update(const TickData &data)
{
  // TODO: We should not convert to real time here
  const auto updated = m_value + m_regenPerSecond * data.elapsed.toSeconds();
  m_value            = std::clamp(updated, m_min, m_max);
}

auto RegenerativeComponent::min() const -> float
{
  return m_min;
}

auto RegenerativeComponent::value() const -> float
{
  return m_value;
}

auto RegenerativeComponent::max() const -> float
{
  return m_max;
}

void RegenerativeComponent::overrideValue(const float value)
{
  m_value = value;
}

void RegenerativeComponent::updateValue(const float delta)
{
  m_value += delta;
}

void RegenerativeComponent::validate()
{
  if (m_max < m_min)
  {
    throw std::invalid_argument("Expected min (" + std::to_string(m_min)
                                + ") to be smaller than max (" + std::to_string(m_max) + ")");
  }
  if (m_value < m_min)
  {
    throw std::invalid_argument("Expected min (" + std::to_string(m_min)
                                + ") to be smaller than value (" + std::to_string(m_value) + ")");
  }
  if (m_max < m_value)
  {
    throw std::invalid_argument("Expected value (" + std::to_string(m_value)
                                + ") to be smaller than max (" + std::to_string(m_max) + ")");
  }
  if (m_regenPerSecond < 0.0f)
  {
    throw std::invalid_argument("Expected regeneration (" + std::to_string(m_regenPerSecond)
                                + ") to be greater than 0");
  }
}

} // namespace bsgo
