
#pragma once

#include "AbstractMessageListener.hh"
#include "AbstractUiHandler.hh"
#include "CredentialsUiHandler.hh"
#include "GameRole.hh"
#include "LoginMessage.hh"
#include "SignupMessage.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "UiTimedMenu.hh"
#include "Views.hh"

namespace bsgalone::client {

class LoginUiHandler : public IUiHandler
{
  public:
  LoginUiHandler();
  ~LoginUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
};

} // namespace bsgalone::client
