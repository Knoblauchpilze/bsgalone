
#pragma once

#include "CoordinateFrame.hh"
#include "CoreObject.hh"
#include "RenderState.hh"
#include "Renderer.hh"
#include "RenderingPass.hh"
#include <memory>

namespace pge {

class IRenderer : public core::CoreObject
{
  public:
  IRenderer(const std::string &name);
  ~IRenderer() override = default;

  virtual void loadResources(int width, int height, sprites::TexturePack &texturesLoader) = 0;
  virtual void render(Renderer &engine,
                      const RenderState &state,
                      const RenderingPass pass) const                                     = 0;
};

using IRendererPtr = std::unique_ptr<IRenderer>;

} // namespace pge
