
#include "FactionComponent.hh"

namespace bsgo {

FactionComponent::FactionComponent(const Faction &faction)
  : AbstractComponent(ComponentType::FACTION)
  , m_faction(faction)
{}

auto FactionComponent::faction() const noexcept -> Faction
{
  return m_faction;
}

void FactionComponent::update(const TickData & /*data*/) {}

} // namespace bsgo
