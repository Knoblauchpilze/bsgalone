
#include "KindComponent.hh"

namespace bsgo {

KindComponent::KindComponent(const EntityKind &kind)
  : AbstractComponent(ComponentType::KIND)
  , m_kind(kind)
{}

auto KindComponent::kind() const noexcept -> EntityKind
{
  return m_kind;
}

void KindComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
