
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class LogoutEvent : public IUiEvent
{
  public:
  LogoutEvent();
  ~LogoutEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
