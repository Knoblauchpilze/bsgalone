
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "LoadingTransition.hh"
#include "Renderer.hh"
#include "TexturePack.hh"
#include "UserInputData.hh"
#include "Views.hh"
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

  virtual void onLoadingTransition(const bsgo::LoadingTransition transition);
  virtual void connectToMessageQueue(bsgo::IMessageQueue &messageQueue);
};

using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

} // namespace pge
