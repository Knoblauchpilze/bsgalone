
#include "LogoutEvent.hh"

namespace bsgalone::client {

LogoutEvent::LogoutEvent()
  : IUiEvent(UiEventType::LOGOUT)
{}

auto LogoutEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<LogoutEvent>();
}

} // namespace bsgalone::client
