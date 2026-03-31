
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class LoginFailedEvent : public IUiEvent
{
  public:
  LoginFailedEvent();
  ~LoginFailedEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
