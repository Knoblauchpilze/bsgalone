
#include "UndockEvent.hh"

namespace bsgalone::client {

UndockEvent::UndockEvent()
  : IUiEvent(UiEventType::UNDOCK)
{}

auto UndockEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<UndockEvent>();
}

} // namespace bsgalone::client
