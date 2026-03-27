
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

  auto getCredentials() const -> const PlayerCredentials &;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ui::UiMenuPtr m_credentialsPanel{};

  PlayerCredentials m_credentials{};

  void onUsernameChanged(const std::string &username);
  void onPasswordChanged(const std::string &password);
};

} // namespace bsgalone::client
