
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ShipView : public utils::CoreObject
{
  public:
  ShipView();
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
