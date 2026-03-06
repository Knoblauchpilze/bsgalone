
#include "DbComponent.hh"

namespace bsgalone::core {

DbComponent::DbComponent(const Uuid dbId)
  : AbstractComponent(ComponentType::DB)
  , m_dbId(dbId)
{}

auto DbComponent::dbId() const noexcept -> Uuid
{
  return m_dbId;
}

void DbComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgalone::core
