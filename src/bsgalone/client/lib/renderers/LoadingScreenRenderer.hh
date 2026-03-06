
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class LoadingScreenRenderer : public DecalScreenRenderer
{
  public:
  LoadingScreenRenderer();
  ~LoadingScreenRenderer() override = default;
};

} // namespace bsgalone::client
