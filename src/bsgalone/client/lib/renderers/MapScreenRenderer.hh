
#pragma once

#include "DecalScreenRenderer.hh"

namespace pge {

class MapScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  MapScreenRenderer();
  ~MapScreenRenderer() override = default;
};

} // namespace pge
