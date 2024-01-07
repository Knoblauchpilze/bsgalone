
#pragma once

#include "IUiHandler.hh"
#include "PlayerCredentials.hh"
#include "UiMenu.hh"
#include "UiTextField.hh"

namespace pge {

class CredentialsUiHandler : public IUiHandler
{
  public:
  CredentialsUiHandler();
  ~CredentialsUiHandler() override = default;

  auto getCredentials() const -> PlayerCredentials;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  UiMenuPtr m_credentialsPanel{};
  UiTextField *m_nameTextField{};
  UiTextField *m_passwordTextField{};
};

} // namespace pge
