
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class ShopReadyEvent : public IUiEvent
{
  public:
  ShopReadyEvent();
  ~ShopReadyEvent() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
