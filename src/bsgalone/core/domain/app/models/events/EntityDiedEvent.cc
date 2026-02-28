
#include "EntityDiedEvent.hh"

namespace bsgalone::core {

EntityDiedEvent::EntityDiedEvent(const bsgo::Uuid entityId)
  : IGameEvent(GameEventType::ENTITY_DIED)
  , m_entityId(entityId)
{}

auto EntityDiedEvent::entityId() const -> bsgo::Uuid
{
  return m_entityId;
}

auto EntityDiedEvent::clone() const -> IGameEventPtr
{
  return std::make_unique<EntityDiedEvent>(m_entityId);
}

} // namespace bsgalone::core
