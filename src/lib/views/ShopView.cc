
#include "ShopView.hh"

namespace bsgo {

ShopView::ShopView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("shop", coordinator, repositories)
{}

} // namespace bsgo
