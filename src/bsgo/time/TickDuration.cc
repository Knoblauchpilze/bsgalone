
#include "TickDuration.hh"
#include <stdexcept>

namespace bsgo {

TickDuration::TickDuration(const float elapsed)
  : m_elapsed(elapsed)
{
  validate();
}

auto TickDuration::str() const -> std::string
{
  return std::to_string(m_elapsed);
}

void TickDuration::validate()
{
  if (m_elapsed < 0.0f)
  {
    throw std::invalid_argument("Tick duration cannot be negative, got: "
                                + std::to_string(m_elapsed));
  }
}

} // namespace bsgo
