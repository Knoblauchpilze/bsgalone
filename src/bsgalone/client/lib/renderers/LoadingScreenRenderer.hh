
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class LoadingScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  LoadingScreenRenderer();
  ~LoadingScreenRenderer() override = default;
};

} // namespace bsgalone::client
