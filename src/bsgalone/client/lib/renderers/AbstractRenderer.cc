
#include "AbstractRenderer.hh"

namespace bsgalone::client {

AbstractRenderer::AbstractRenderer(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("renderer");
}

} // namespace bsgalone::client
