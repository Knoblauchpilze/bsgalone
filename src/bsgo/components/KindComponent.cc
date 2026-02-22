
#include "KindComponent.hh"

namespace bsgo {

KindComponent::KindComponent(const bsgalone::core::EntityKind &kind)
  : AbstractComponent(ComponentType::KIND)
  , m_kind(kind)
{}

auto KindComponent::kind() const noexcept -> bsgalone::core::EntityKind
{
  return m_kind;
}

void KindComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
