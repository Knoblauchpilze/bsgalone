
#pragma once

#include "DecalScreenRenderer.hh"

namespace pge {

class LoadingScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  LoadingScreenRenderer();
  ~LoadingScreenRenderer() override = default;
};

} // namespace pge
