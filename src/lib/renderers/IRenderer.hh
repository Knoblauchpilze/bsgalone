
#pragma once

#include "Action.hh"
#include "Controls.hh"
#include "InputHandle.hh"
#include "RenderState.hh"
#include "olcPixelGameEngine.h"
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

  virtual void render(olc::PixelGameEngine *pge, const RenderState &state) const = 0;
  virtual auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle = 0;
};

using IRendererPtr = std::unique_ptr<IRenderer>;

} // namespace pge