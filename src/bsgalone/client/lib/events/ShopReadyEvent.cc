
#include "ShopReadyEvent.hh"

namespace bsgalone::client {

ShopReadyEvent::ShopReadyEvent()
  : IUiEvent(UiEventType::SHOP_READY)
{}

auto ShopReadyEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<ShopReadyEvent>();
}

} // namespace bsgalone::client
