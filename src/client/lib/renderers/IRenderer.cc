
#include "IRenderer.hh"

namespace pge {

IRenderer::IRenderer(const std::string &name)
  : core::CoreObject(name)
{
  setService("renderer");
}

} // namespace pge
