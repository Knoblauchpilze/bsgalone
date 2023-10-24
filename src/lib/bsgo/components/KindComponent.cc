
#include "KindComponent.hh"

namespace bsgo {

KindComponent::KindComponent(const EntityKind &kind)
  : IComponent("kind")
  , m_kind(kind)
{}

auto KindComponent::kind() const noexcept -> EntityKind
{
  return m_kind;
}

void KindComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
