
#include "RenderingPass.hh"

namespace pge {

auto str(const RenderingPass pass) -> std::string
{
  switch (pass)
  {
    case RenderingPass::SPRITES:
      return "sprites";
    case RenderingPass::DECAL:
      return "decal";
    case RenderingPass::UI:
      return "ui";
    case RenderingPass::DEBUG:
      return "debug";
    default:
      return "unknown";
  }
}

} // namespace pge
