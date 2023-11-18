
#pragma once

#include "Action.hh"
#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "Game.hh"
#include "UserInputData.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class Action;
using ActionShPtr = std::shared_ptr<Action>;

class IUiHandler : public utils::CoreObject
{
  public:
  IUiHandler(const std::string &name);
  ~IUiHandler() override = default;

  virtual void initializeMenus(const int width, const int height) = 0;
  virtual bool processUserInput(UserInputData &inputData)         = 0;
  virtual void render(SpriteRenderer &engine) const               = 0;
  virtual void updateUi()                                         = 0;
};

using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

} // namespace pge
