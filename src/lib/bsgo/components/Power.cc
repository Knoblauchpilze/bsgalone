
#include "Power.hh"

namespace bsgo {

Power::Power(const float current, const float max, const std::optional<float> min)
  : m_current(current)
  , m_max(max)
{
  if (min)
  {
    m_min = *min;
  }
  validate();
}

auto Power::power() const -> float
{
  return m_current;
}

auto Power::max() const -> float
{
  return m_max;
}

void Power::validate()
{
  if (m_min > m_current)
  {
    throw std::invalid_argument("Expected min (" + std::to_string(m_min)
                                + ") to be lower than current (" + std::to_string(m_current) + ")");
  }
  if (m_current > m_max)
  {
    throw std::invalid_argument("Expected current (" + std::to_string(m_current)
                                + ") to be lower than max (" + std::to_string(m_max) + ")");
  }
  if (m_min > m_max)
  {
    throw std::invalid_argument("Expected min (" + std::to_string(m_min)
                                + ") to be lower than max (" + std::to_string(m_max) + ")");
  }
}

} // namespace bsgo
