
#include "SlotSystem.hh"

namespace bsgo {

SlotSystem::SlotSystem()
  : ISystem("weapon")
{}

void SlotSystem::update(const Components &components, const float elapsedSeconds)
{
  for (const auto &[_, weapon] : components.weapons)
  {
    weapon->update(elapsedSeconds);
  }
}

} // namespace bsgo
