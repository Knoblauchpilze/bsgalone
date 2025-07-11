
#pragma once

#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "CoreObject.hh"
#include "Game.hh"
#include "IMessageQueue.hh"
#include "UserInputData.hh"
#include <memory>

namespace pge {

class IUiHandler : public core::CoreObject
{
  public:
  IUiHandler(const std::string &name);
  ~IUiHandler() override = default;

  virtual void initializeMenus(const int width,
                               const int height,
                               sprites::TexturePack &texturesLoader)
    = 0;
  virtual bool processUserInput(UserInputData &inputData) = 0;
  virtual void render(Renderer &engine) const             = 0;
  virtual void updateUi()                                 = 0;

  virtual void reset();
  virtual void connectToMessageQueue(bsgo::IMessageQueue &messageQueue);
};

using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

} // namespace pge
