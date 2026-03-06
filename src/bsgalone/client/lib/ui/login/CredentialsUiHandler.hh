
#pragma once

#include "AbstractUiHandler.hh"
#include "PlayerCredentials.hh"
#include "UiMenu.hh"
#include "UiTextField.hh"

namespace bsgalone::client {

class CredentialsUiHandler : public AbstractUiHandler
{
  public:
  CredentialsUiHandler();
  ~CredentialsUiHandler() override = default;

  auto getCredentials() const -> PlayerCredentials;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ui::UiMenuPtr m_credentialsPanel{};
  ui::UiTextField *m_nameTextField{};
  ui::UiTextField *m_passwordTextField{};
};

} // namespace bsgalone::client
