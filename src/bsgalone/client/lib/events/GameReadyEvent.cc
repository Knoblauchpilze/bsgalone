
#include "GameReadyEvent.hh"

namespace bsgalone::client {

GameReadyEvent::GameReadyEvent(const std::string &systemName)
  : IUiEvent(UiEventType::GAME_READY)
  , m_systemName(systemName)
{}

auto GameReadyEvent::getSystemName() const -> std::string
{
  return m_systemName;
}

auto GameReadyEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<GameReadyEvent>(m_systemName);
}

} // namespace bsgalone::client
