
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class GameReadyEvent : public IUiEvent
{
  public:
  GameReadyEvent();
  ~GameReadyEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
