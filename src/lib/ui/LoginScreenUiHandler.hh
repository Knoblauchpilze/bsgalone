
#pragma once

#include "IUiHandler.hh"
#include "Menu.hh"
#include "UiMenu.hh"

namespace pge {

class LoginScreenUiHandler : public IUiHandler
{
  public:
  LoginScreenUiHandler();
  ~LoginScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  UiMenuPtr m_loginPanel{};
  UiMenuPtr m_loginButton{};
  UiMenuPtr m_quitButton{};

  void generateLoginMenu(const int width, const int height);
  void generateLoginButton(const int width, const int height);
  void generateQuitMenu(const int width, const int height);
};

} // namespace pge
