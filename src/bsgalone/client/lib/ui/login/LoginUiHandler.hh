
#pragma once

#include "CoreObject.hh"
#include "CredentialsUiHandler.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "IUiCommandQueue.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "UiTimedMenu.hh"

namespace bsgalone::client {

/// @brief - Forward declaration of the proxy class allowing the handler
/// to listen to UI events. This allows to make this class a friend class
/// of the handler.
class UiEventListenerLoginProxy;

class LoginUiHandler : public IUiHandler, public ::core::CoreObject
{
  public:
  LoginUiHandler(IUiEventQueueShPtr inputQueue, IUiCommandQueueShPtr outputQueue);
  ~LoginUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
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

  IUiCommandQueueShPtr m_queue{};

  void registerToQueue(IUiEventQueueShPtr inputQueue);

  void generateLoginModePanel(const pge::Vec2i &dimensions);
  void generateFactionPanel(const pge::Vec2i &dimensions);
  void generateRolePanel(const pge::Vec2i &dimensions);
  void generateProceedButton(const pge::Vec2i &dimensions);
  void generateQuitButton(const pge::Vec2i &dimensions);
  void generateFailureMenu(const pge::Vec2i &dimensions);
  void generateSuccessfulSignupMenu(const pge::Vec2i &dimensions);

  void setLoginMode(const Mode mode);
  void setFaction(const core::Faction faction);
  void setGameRole(const core::GameRole role);

  void onProceedRequested();
  void onExitRequested();
  void onLoginFailed();
  void onSignupSucceeded();
  void onSignupFailed();

  friend class UiEventListenerLoginProxy;
};

} // namespace bsgalone::client
