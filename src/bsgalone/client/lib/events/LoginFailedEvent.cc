
#include "LoginFailedEvent.hh"

namespace bsgalone::client {

LoginFailedEvent::LoginFailedEvent()
  : IUiEvent(UiEventType::LOGIN_FAILED)
{}

auto LoginFailedEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<LoginFailedEvent>();
}

} // namespace bsgalone::client
