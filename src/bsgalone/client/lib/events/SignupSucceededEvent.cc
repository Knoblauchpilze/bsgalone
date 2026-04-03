
#include "SignupSucceededEvent.hh"

namespace bsgalone::client {

SignupSucceededEvent::SignupSucceededEvent()
  : IUiEvent(UiEventType::SIGNUP_SUCCEEDED)
{}

auto SignupSucceededEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<SignupSucceededEvent>();
}

} // namespace bsgalone::client
