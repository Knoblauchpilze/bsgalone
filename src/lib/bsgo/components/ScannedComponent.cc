
#include "ScannedComponent.hh"

namespace bsgo {

ScannedComponent::ScannedComponent()
  : IComponent("scanned")
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

void ScannedComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
