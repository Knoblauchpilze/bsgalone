
#include "JumpCompletedEvent.hh"

namespace bsgalone::core {

JumpCompletedEvent::JumpCompletedEvent(const bsgo::Uuid entityId)
  : IGameEvent(GameEventType::JUMP_COMPLETED)
  , m_entityId(entityId)
{}

auto JumpCompletedEvent::entityId() const -> bsgo::Uuid
{
  return m_entityId;
}

auto JumpCompletedEvent::clone() const -> IGameEventPtr
{
  return std::make_unique<JumpCompletedEvent>(m_entityId);
}

} // namespace bsgalone::core
