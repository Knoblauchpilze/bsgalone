
#include "RegenerativeComponent.hh"

namespace bsgo {

RegenerativeComponent::RegenerativeComponent(const std::string &name,
                                             const float min,
                                             const float value,
                                             const float max,
                                             const float regenPerSecond)
  : IComponent(name)
  , m_min(min)
  , m_value(value)
  , m_max(max)
  , m_regenPerSecond(regenPerSecond)
{
  validate();
}

void RegenerativeComponent::update(const float elapsedSeconds)
{
  const auto updated = m_value + m_regenPerSecond * elapsedSeconds;
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

void RegenerativeComponent::use(const float amount)
{
  if (m_value < amount)
  {
    error("Can't use " + std::to_string(amount), "Only " + std::to_string(m_value) + " available");
  }
  m_value -= amount;
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
