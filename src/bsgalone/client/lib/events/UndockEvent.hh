
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class UndockEvent : public IUiEvent
{
  public:
  UndockEvent();
  ~UndockEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
