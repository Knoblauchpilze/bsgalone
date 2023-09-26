
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class MapView : public utils::CoreObject
{
  public:
  MapView();
};

using MapViewPtr = std::unique_ptr<MapView>;

} // namespace bsgo
