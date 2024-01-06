
#include "ShipClassComponent.hh"

namespace bsgo {

ShipClassComponent::ShipClassComponent(const ShipClass &shipClass)
  : AbstractComponent(ComponentType::SHIP_CLASS)
  , m_shipClass(shipClass)
{}

auto ShipClassComponent::shipClass() const noexcept -> ShipClass
{
  return m_shipClass;
}

void ShipClassComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
