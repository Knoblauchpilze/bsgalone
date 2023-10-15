
#include "PowerSystem.hh"

namespace bsgo {

PowerSystem::PowerSystem()
  : ISystem("power")
{}

void PowerSystem::update(const Components &components, const float elapsedSeconds)
{
  for (const auto &[_, power] : components.powers)
  {
    power->update(elapsedSeconds);
  }
}

} // namespace bsgo
