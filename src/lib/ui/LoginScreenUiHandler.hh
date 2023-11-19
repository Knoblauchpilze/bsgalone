
#pragma once

#include "IUiHandler.hh"
#include "Menu.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"

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

  private:
  bsgo::PlayerViewShPtr m_playerView;

  UiMenuPtr m_loginPanel{};
  UiTextMenu *m_nameTextField{};
  UiTextMenu *m_passwordTextField{};
  UiTextMenuPtr m_loginButton{};
  UiTextMenuPtr m_quitButton{};

  void generateLoginMenu(const int width, const int height);
  void generateLoginButton(const int width, const int height);
  void generateQuitMenu(const int width, const int height);

  void tryLogin(Game &game);
};

} // namespace pge
