
#pragma once

#include "IUiHandler.hh"
#include "PlayerCredentials.hh"
#include "UiMenu.hh"
#include "UiTextField.hh"

namespace bsgalone::client {

class CredentialsUiHandler : public IUiHandler
{
  public:
  CredentialsUiHandler();
  ~CredentialsUiHandler() override = default;

  // TODO: Maybe the text fields could have a onChanged callback in the
  // config which would allow to not have to keep raw pointers.
  auto getCredentials() const -> PlayerCredentials;

  void initializeMenus(const pge::Vec2i &dimensions,
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
