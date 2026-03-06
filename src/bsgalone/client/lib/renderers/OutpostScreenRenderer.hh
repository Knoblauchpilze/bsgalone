
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class OutpostScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  OutpostScreenRenderer();
  ~OutpostScreenRenderer() override = default;
};

} // namespace bsgalone::client
