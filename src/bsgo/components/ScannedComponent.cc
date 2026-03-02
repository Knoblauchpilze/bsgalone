
#include "ScannedComponent.hh"

namespace bsgo {

ScannedComponent::ScannedComponent()
  : AbstractComponent(bsgalone::core::ComponentType::SCANNED)
{}

bool ScannedComponent::scanned() const
{
  return m_scanned;
}

void ScannedComponent::scan()
{
  m_scanned = true;
}

void ScannedComponent::reset()
{
  m_scanned = false;
}

void ScannedComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
