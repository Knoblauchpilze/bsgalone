
#include "HullPoints.hh"

namespace bsgo {

HullPoints::HullPoints(const float current, const float max)
  : m_current(current)
  , m_max(max)
{
  validate();
}

auto HullPoints::power() const -> float
{
  return m_current;
}

auto HullPoints::max() const -> float
{
  return m_max;
}

void HullPoints::validate()
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
