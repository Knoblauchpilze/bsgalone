
#include "DbComponent.hh"

namespace bsgo {

DbComponent::DbComponent(const Uuid dbId)
  : AbstractComponent(bsgalone::core::ComponentType::DB)
  , m_dbId(dbId)
{}

auto DbComponent::dbId() const noexcept -> Uuid
{
  return m_dbId;
}

void DbComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
