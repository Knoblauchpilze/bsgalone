
#include "SlotSystem.hh"

namespace bsgo {

SlotSystem::SlotSystem()
  : ISystem("slot")
{}

void SlotSystem::update(const Components &components,
                        const Coordinator & /*coordinator*/,
                        const float elapsedSeconds)
{
  for (const auto &[_, weapon] : components.weapons)
  {
    weapon->update(elapsedSeconds);
  }
}

} // namespace bsgo
