
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

class LoginScreenUiHandler : public AbstractUiHandler, public core::AbstractMessageListener
{
  public:
  LoginScreenUiHandler(const Views &views);
  ~LoginScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  void connectToMessageQueue(core::IMessageQueue &messageQueue) override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  PlayerViewShPtr m_playerView{};

  enum class Mode
  {
    LOGIN,
    SIGNUP,
  };
  Mode m_mode{Mode::LOGIN};
  core::Faction m_faction{core::Faction::COLONIAL};
  core::GameRole m_role{core::GameRole::PILOT};

  ui::UiMenuPtr m_loginModePanel{};
  ui::UiTextMenu *m_loginButton{};
  ui::UiTextMenu *m_signupButton{};

  ui::UiMenuPtr m_factionPanel{};
  ui::UiTextMenu *m_colonialButton{};
  ui::UiTextMenu *m_cylonButton{};

  ui::UiMenuPtr m_rolePanel{};
  ui::UiTextMenu *m_pilotButton{};
  ui::UiTextMenu *m_gunnerButton{};

  CredentialsUiHandler m_credentialsUiHandler{};
  ui::UiTextMenuPtr m_proceedButton{};
  ui::UiTextMenuPtr m_quitButton{};

  ui::UiTimedMenuPtr m_failureMenu{};
  ui::UiTextMenu *m_failureMenuText{};

  ui::UiTimedMenuPtr m_successfulSignupMenu{};

  void generateLoginModePanel(const int width, const int height);
  void generateFactionPanel(const int width, const int height);
  void generateRolePanel(const int width, const int height);
  void generateProceedButton(const int width, const int height);
  void generateQuitButton(const int width, const int height);
  void generateFailureMenu(const int width, const int height);
  void generateSuccessfulSignupMenu(const int width, const int height);

  void setLoginMode(const Mode mode);
  void setFaction(const core::Faction faction);
  void setGameRole(const core::GameRole role);
  void tryLogin();

  void handleLoginMessage(const core::LoginMessage &message);
  void handleSignupMessage(const core::SignupMessage &message);
};

} // namespace bsgalone::client
