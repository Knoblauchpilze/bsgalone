
#pragma once

#include "DecalScreenRenderer.hh"

namespace pge {

class LoginScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  LoginScreenRenderer();
  ~LoginScreenRenderer() override = default;
};

} // namespace pge
