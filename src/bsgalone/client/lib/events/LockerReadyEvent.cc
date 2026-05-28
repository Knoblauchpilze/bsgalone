
#include "LockerReadyEvent.hh"

namespace bsgalone::client {

LockerReadyEvent::LockerReadyEvent()
  : IUiEvent(UiEventType::LOCKER_READY)
{}

auto LockerReadyEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<LockerReadyEvent>();
}

} // namespace bsgalone::client
