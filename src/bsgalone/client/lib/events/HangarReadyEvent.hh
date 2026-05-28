
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class HangarReadyEvent : public IUiEvent
{
  public:
  HangarReadyEvent();
  ~HangarReadyEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
