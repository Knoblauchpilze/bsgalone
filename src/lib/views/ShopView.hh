
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShopView : public utils::CoreObject
{
  public:
  ShopView();
};

using ShopViewPtr = std::unique_ptr<ShopView>;

} // namespace bsgo
