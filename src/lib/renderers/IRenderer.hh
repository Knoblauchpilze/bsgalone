
#pragma once

#include "Action.hh"
#include "Controls.hh"
#include "InputHandle.hh"
#include "RenderState.hh"
#include "RenderingPass.hh"
#include "SpriteRenderer.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class Action;
using ActionShPtr = std::shared_ptr<Action>;

class IRenderer : public utils::CoreObject
{
  public:
  IRenderer();
  virtual ~IRenderer() = default;

  virtual void loadResources(int width, int height, sprites::TexturePack &texturesLoader) = 0;
  virtual void render(SpriteRenderer &engine,
                      const RenderState &state,
                      const RenderingPass pass) const                                     = 0;
  virtual auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle  = 0;
  virtual void updateUi() = 0;
};

using IRendererPtr = std::unique_ptr<IRenderer>;

} // namespace pge