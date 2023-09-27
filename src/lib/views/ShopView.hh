
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShopView : public utils::CoreObject
{
  public:
  ShopView();
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
