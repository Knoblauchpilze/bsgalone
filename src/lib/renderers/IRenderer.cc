
#include "IRenderer.hh"

namespace pge {

IRenderer::IRenderer()
  : utils::CoreObject("renderer")
{
  setService("bsgo");
}

} // namespace pge