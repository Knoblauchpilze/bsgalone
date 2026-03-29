
#include "CredentialsUiHandler.hh"
#include "ScreenCommon.hh"

namespace bsgalone::client {

CredentialsUiHandler::CredentialsUiHandler()
  : IUiHandler()
{}

auto CredentialsUiHandler::getCredentials() const -> const PlayerCredentials &
{
  return m_credentials;
}

namespace {
void addTextFieldSectionToMenu(ui::UiMenu &mainPanel,
                               const std::string &textFieldlabel,
                               ui::TextChangedCallback callback,
                               const std::optional<std::string> &defaultValue)
{
  auto textFieldSection = generateBlankHorizontalMenu();

  const ui::MenuConfig config{.highlightable = false};
  auto bg    = ui::bgConfigFromColor(semiOpaque(pge::colors::DARK_BLUE));
  auto text  = ui::textConfigFromColor(textFieldlabel, pge::colors::GREY, ui::TextAlignment::RIGHT);
  auto label = std::make_unique<ui::UiTextMenu>(config, bg, text);
  textFieldSection->addMenu(std::move(label));

  ui::TextFieldConfig fieldConfig{.textChangedCallback = callback};
  bg               = ui::bgConfigFromColor(semiOpaque(pge::colors::WHITE));
  const auto value = defaultValue.value_or("");
  text             = ui::textConfigFromColor(value, pge::colors::BLACK, ui::TextAlignment::LEFT);

  auto field = std::make_unique<ui::UiTextField>(fieldConfig, bg, text);
  textFieldSection->addMenu(std::move(field));

  mainPanel.addMenu(std::move(textFieldSection));
}
} // namespace

void CredentialsUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                           pge::sprites::TexturePack & /*texturesLoader*/)
{
  const pge::Vec2i loginDimsPixels{400, 200};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 200;
  const pge::Vec2i loginPos{(dimensions.x - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  m_credentialsPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  auto userCallback = [this](const std::string &text) { onUsernameChanged(text); };
  addTextFieldSectionToMenu(*m_credentialsPanel, "Name:", userCallback, "colo");

  auto pwdCallback = [this](const std::string &text) { onPasswordChanged(text); };
  addTextFieldSectionToMenu(*m_credentialsPanel, "Password:", pwdCallback, "aze");

  m_credentials.username = "colo";
  m_credentials.password = "aze";
}

bool CredentialsUiHandler::processUserInput(ui::UserInputData &inputData)
{
  return m_credentialsPanel->processUserInput(inputData);
}

void CredentialsUiHandler::render(pge::Renderer &engine) const
{
  m_credentialsPanel->render(engine);
}

void CredentialsUiHandler::updateUi()
{
  // Intentionally empty
}

void CredentialsUiHandler::onUsernameChanged(const std::string &username)
{
  m_credentials.username = username;
}

void CredentialsUiHandler::onPasswordChanged(const std::string &password)
{
  m_credentials.password = password;
}

} // namespace bsgalone::client
