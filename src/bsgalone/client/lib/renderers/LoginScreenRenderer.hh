
#pragma once

#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

class LoginScreenRenderer : public bsgalone::client::DecalScreenRenderer
{
  public:
  LoginScreenRenderer();
  ~LoginScreenRenderer() override = default;
};

} // namespace bsgalone::client
