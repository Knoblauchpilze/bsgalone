
#include "AbstractRenderer.hh"

namespace pge {

AbstractRenderer::AbstractRenderer(const std::string &name)
  : core::CoreObject(name)
{
  setService("renderer");
}

} // namespace pge
