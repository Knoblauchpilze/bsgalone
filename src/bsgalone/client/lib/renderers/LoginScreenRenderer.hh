
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class LoginScreenRenderer : public DecalScreenRenderer
{
  public:
  LoginScreenRenderer();
  ~LoginScreenRenderer() override = default;
};

} // namespace bsgalone::client
