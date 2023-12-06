
#pragma once

#include "CredentialsUiHandler.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "UiTimedMenu.hh"

namespace pge {

class LoginScreenUiHandler : public IUiHandler
{
  public:
  LoginScreenUiHandler(const bsgo::Views &views);
  ~LoginScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::LoginViewShPtr m_loginView;

  enum class Mode
  {
    LOGIN,
    SIGNUP,
  };
  Mode m_mode{Mode::LOGIN};
  bsgo::Faction m_faction{bsgo::Faction::COLONIAL};

  UiMenuPtr m_loginModePanel{};
  UiTextMenu *m_loginButton{};
  UiTextMenu *m_signupButton{};

  UiMenuPtr m_factionPanel{};

  CredentialsUiHandler m_credentialsUiHandler{};
  UiTextMenuPtr m_proceedButton{};
  UiTextMenuPtr m_quitButton{};

  UiTimedMenuPtr m_failureMenu{};
  UiTextMenu *m_failureMenuText{};

  void generateLoginModePanel(const int width, const int height);
  void generateFactionPanel(const int width, const int height);
  void generateProceedButton(const int width, const int height);
  void generateQuitButton(const int width, const int height);
  void generateFailureMenu(const int width, const int height);

  void setLoginMode(const Mode &mode);
  void setFaction(const bsgo::Faction &faction);
  void tryLogin(Game &game);
};

} // namespace pge
