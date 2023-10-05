
#include "ShopView.hh"

namespace bsgo {

ShopView::ShopView()
  : utils::CoreObject("shop")
{
  setService("view");
}

void ShopView::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
