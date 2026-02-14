
#pragma once

#include "AbstractMessageListener.hh"
#include "CredentialsUiHandler.hh"
#include "GameRole.hh"
#include "IUiHandler.hh"
#include "LoginMessage.hh"
#include "SignupMessage.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "UiTimedMenu.hh"
#include "Views.hh"

namespace pge {

class LoginScreenUiHandler : public IUiHandler, public bsgo::AbstractMessageListener
{
  public:
  LoginScreenUiHandler(const Views &views);
  ~LoginScreenUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  void connectToMessageQueue(bsgo::IMessageQueue &messageQueue) override;
  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  PlayerViewShPtr m_playerView{};

  enum class Mode
  {
    LOGIN,
    SIGNUP,
  };
  Mode m_mode{Mode::LOGIN};
  bsgo::Faction m_faction{bsgo::Faction::COLONIAL};
  bsgo::GameRole m_role{bsgo::GameRole::PILOT};

  UiMenuPtr m_loginModePanel{};
  UiTextMenu *m_loginButton{};
  UiTextMenu *m_signupButton{};

  UiMenuPtr m_factionPanel{};
  UiTextMenu *m_colonialButton{};
  UiTextMenu *m_cylonButton{};

  UiMenuPtr m_rolePanel{};
  UiTextMenu *m_pilotButton{};
  UiTextMenu *m_gunnerButton{};

  CredentialsUiHandler m_credentialsUiHandler{};
  UiTextMenuPtr m_proceedButton{};
  UiTextMenuPtr m_quitButton{};

  UiTimedMenuPtr m_failureMenu{};
  UiTextMenu *m_failureMenuText{};

  UiTimedMenuPtr m_successfulSignupMenu{};

  void generateLoginModePanel(const int width, const int height);
  void generateFactionPanel(const int width, const int height);
  void generateRolePanel(const int width, const int height);
  void generateProceedButton(const int width, const int height);
  void generateQuitButton(const int width, const int height);
  void generateFailureMenu(const int width, const int height);
  void generateSuccessfulSignupMenu(const int width, const int height);

  void setLoginMode(const Mode mode);
  void setFaction(const bsgo::Faction faction);
  void setGameRole(const bsgo::GameRole role);
  void tryLogin();

  void handleLoginMessage(const bsgo::LoginMessage &message);
  void handleSignupMessage(const bsgo::SignupMessage &message);
};

} // namespace pge
