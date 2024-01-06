
#include "IRenderer.hh"

namespace pge {

IRenderer::IRenderer(const std::string &name)
  : utils::CoreObject(name)
{
  setService("renderer");
}

} // namespace pge
