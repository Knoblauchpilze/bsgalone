
#include "SignupFailedEvent.hh"

namespace bsgalone::client {

SignupFailedEvent::SignupFailedEvent()
  : IUiEvent(UiEventType::SIGNUP_FAILED)
{}

auto SignupFailedEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<SignupFailedEvent>();
}

} // namespace bsgalone::client
