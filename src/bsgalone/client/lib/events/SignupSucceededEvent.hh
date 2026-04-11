
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class SignupSucceededEvent : public IUiEvent
{
  public:
  SignupSucceededEvent();
  ~SignupSucceededEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
