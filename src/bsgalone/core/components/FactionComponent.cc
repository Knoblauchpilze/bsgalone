
#include "FactionComponent.hh"

namespace bsgalone::core {

FactionComponent::FactionComponent(const Faction &faction)
  : AbstractComponent(ComponentType::FACTION)
  , m_faction(faction)
{}

auto FactionComponent::faction() const noexcept -> Faction
{
  return m_faction;
}

void FactionComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgalone::core
