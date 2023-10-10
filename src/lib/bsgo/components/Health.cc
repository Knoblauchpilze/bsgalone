
#include "Health.hh"

namespace bsgo {

Health::Health(const float current, const float max)
  : m_current(current)
  , m_max(max)
{
  validate();
}

auto Health::health() const -> float
{
  return m_current;
}

auto Health::max() const -> float
{
  return m_max;
}

void Health::validate()
{
  if (m_current > m_max)
  {
    throw std::invalid_argument("Expected current (" + std::to_string(m_current)
                                + ") to be lower than max (" + std::to_string(m_max) + ")");
  }
  if (m_current < 0.0f)
  {
    throw std::invalid_argument("Expected current (" + std::to_string(m_current)
                                + " to be at least 0");
  }
}

} // namespace bsgo
