
#include "IGameEvent.hh"

namespace bsgalone::core {

IGameEvent::IGameEvent(const GameEventType type)
  : m_type(type)
{}

auto IGameEvent::type() const -> GameEventType
{
  return m_type;
}

} // namespace bsgalone::core
