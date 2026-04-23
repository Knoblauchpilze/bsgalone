
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class LoadingFinishedEvent : public IUiEvent
{
  public:
  LoadingFinishedEvent();
  ~LoadingFinishedEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
