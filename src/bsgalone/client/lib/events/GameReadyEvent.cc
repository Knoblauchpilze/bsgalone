
#include "GameReadyEvent.hh"

namespace bsgalone::client {

GameReadyEvent::GameReadyEvent()
  : IUiEvent(UiEventType::GAME_READY)
{}

auto GameReadyEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<GameReadyEvent>();
}

} // namespace bsgalone::client
