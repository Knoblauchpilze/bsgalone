
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextField.hh"
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
  bsgo::LoginViewShPtr m_loginView;

  UiMenuPtr m_loginPanel{};
  UiTextField *m_nameTextField{};
  UiTextField *m_passwordTextField{};
  UiTextMenuPtr m_loginButton{};
  UiTextMenuPtr m_quitButton{};

  void generateLoginMenu(const int width, const int height);
  void generateLoginButton(const int width, const int height);
  void generateQuitMenu(const int width, const int height);

  void tryLogin(Game &game);
};

} // namespace pge
