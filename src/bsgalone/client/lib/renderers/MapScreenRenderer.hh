
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class MapScreenRenderer : public DecalScreenRenderer
{
  public:
  MapScreenRenderer();
  ~MapScreenRenderer() override = default;
};

} // namespace bsgalone::client
