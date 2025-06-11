
#pragma once

#include "DecalScreenRenderer.hh"

namespace pge {

class LoadingScreenRenderer : public DecalScreenRenderer
{
  public:
  LoadingScreenRenderer();
  ~LoadingScreenRenderer() override = default;
};

} // namespace pge
