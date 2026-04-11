
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class LoginSucceededEvent : public IUiEvent
{
  public:
  LoginSucceededEvent();
  ~LoginSucceededEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
