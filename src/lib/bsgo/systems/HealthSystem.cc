
#include "HealthSystem.hh"

namespace bsgo {

HealthSystem::HealthSystem()
  : ISystem("health")
{}

void HealthSystem::update(const Components &components, const float elapsedSeconds)
{
  for (const auto &[_, health] : components.healths)
  {
    health->update(elapsedSeconds);
  }
}

} // namespace bsgo
