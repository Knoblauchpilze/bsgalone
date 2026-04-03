
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class SignupFailedEvent : public IUiEvent
{
  public:
  SignupFailedEvent();
  ~SignupFailedEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
