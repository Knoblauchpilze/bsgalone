
#include "LoginSucceededEvent.hh"

namespace bsgalone::client {

LoginSucceededEvent::LoginSucceededEvent()
  : IUiEvent(UiEventType::LOGIN_SUCCEEDED)
{}

auto LoginSucceededEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<LoginSucceededEvent>();
}

} // namespace bsgalone::client
