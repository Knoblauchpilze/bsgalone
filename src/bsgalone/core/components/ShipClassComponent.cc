
#include "ShipClassComponent.hh"

namespace bsgalone::core {

ShipClassComponent::ShipClassComponent(const ShipClass &shipClass)
  : AbstractComponent(ComponentType::SHIP_CLASS)
  , m_shipClass(shipClass)
{}

auto ShipClassComponent::shipClass() const noexcept -> ShipClass
{
  return m_shipClass;
}

void ShipClassComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgalone::core
