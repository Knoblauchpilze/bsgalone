
#include "Duration.hh"
#include <stdexcept>

namespace bsgo {

Duration::Duration(const float elapsed)
  : m_elapsed(elapsed)
{
  validate();
}

auto Duration::str() const -> std::string
{
  return std::to_string(m_elapsed);
}

void Duration::validate()
{
  if (m_elapsed < 0.0f)
  {
    throw std::invalid_argument("Duration cannot be negative, got: " + std::to_string(m_elapsed));
  }
}

} // namespace bsgo
