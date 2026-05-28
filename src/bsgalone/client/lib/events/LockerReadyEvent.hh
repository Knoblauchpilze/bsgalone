
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class LockerReadyEvent : public IUiEvent
{
  public:
  LockerReadyEvent();
  ~LockerReadyEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
