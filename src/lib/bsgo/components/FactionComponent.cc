
#include "FactionComponent.hh"

namespace bsgo {

FactionComponent::FactionComponent(const Faction &faction)
  : IComponent("faction")
  , m_faction(faction)
{}

auto FactionComponent::faction() const noexcept -> Faction
{
  return m_faction;
}

void FactionComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
