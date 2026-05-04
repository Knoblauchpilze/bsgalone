
#include "HealthComponent.hh"

namespace bsgalone::core {

bool HealthComponent::isAlive() const
{
  return value >= 0.0f;
}

} // namespace bsgalone::core
