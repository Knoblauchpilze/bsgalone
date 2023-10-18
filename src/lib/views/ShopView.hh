
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class ShopView : public IView
{
  public:
  ShopView(const CoordinatorShPtr &coordinator);
  ~ShopView() override = default;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
