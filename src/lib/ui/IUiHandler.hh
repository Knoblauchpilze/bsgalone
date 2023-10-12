
#pragma once

#include "Action.hh"
#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "InputHandle.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class Action;
using ActionShPtr = std::shared_ptr<Action>;

class IUiHandler : public utils::CoreObject
{
  public:
  IUiHandler(const std::string &name);
  virtual ~IUiHandler() = default;

  virtual void initializeMenus(const int width, const int height) = 0;
  virtual auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle                            = 0;
  virtual void render(SpriteRenderer &engine) const = 0;
  virtual void updateUi()                           = 0;
};

using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

} // namespace pge
