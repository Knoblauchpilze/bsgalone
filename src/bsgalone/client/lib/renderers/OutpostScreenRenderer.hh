
#pragma once

#include "DecalScreenRenderer.hh"

namespace pge {

class OutpostScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  OutpostScreenRenderer();
  ~OutpostScreenRenderer() override = default;
};

} // namespace pge
