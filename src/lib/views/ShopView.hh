
#pragma once

#include "IView.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShopView : public utils::CoreObject, public IView
{
  public:
  ShopView();
  ~ShopView() override = default;

  void update(const float elapsedSeconds) override;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
