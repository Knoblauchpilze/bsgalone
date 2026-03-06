
#pragma once

#include "CoreObject.hh"
#include "IRenderer.hh"

namespace bsgalone::client {

class AbstractRenderer : public bsgalone::client::IRenderer, public ::core::CoreObject
{
  public:
  AbstractRenderer(const std::string &name);
  ~AbstractRenderer() override = default;
};

} // namespace bsgalone::client
