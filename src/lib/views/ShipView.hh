
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShipView : public utils::CoreObject
{
  public:
  ShipView();
};

using ShipViewPtr = std::unique_ptr<ShipView>;

} // namespace bsgo
