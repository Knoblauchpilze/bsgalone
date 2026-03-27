
#include "CredentialsUiHandler.hh"
#include "ScreenCommon.hh"

namespace bsgalone::client {

CredentialsUiHandler::CredentialsUiHandler()
  : IUiHandler()
{}

auto CredentialsUiHandler::getCredentials() const -> PlayerCredentials
{
  return PlayerCredentials{
    .name     = m_nameTextField->getText(),
    .password = m_passwordTextField->getText(),
  };
}

namespace {
auto addTextFieldSectionToMenu(ui::UiMenu &mainPanel,
                               const std::string &textFieldlabel,
                               const std::optional<std::string> &defaultValue = {})
  -> ui::UiTextField *
{
  auto textFieldSection = generateBlankHorizontalMenu();

  const ui::MenuConfig config{.highlightable = false};
  auto bg    = ui::bgConfigFromColor(semiOpaque(pge::colors::DARK_BLUE));
  auto text  = ui::textConfigFromColor(textFieldlabel, pge::colors::GREY, ui::TextAlignment::RIGHT);
  auto label = std::make_unique<ui::UiTextMenu>(config, bg, text);
  textFieldSection->addMenu(std::move(label));

  ui::TextFieldConfig fieldConfig{};
  bg               = ui::bgConfigFromColor(semiOpaque(pge::colors::WHITE));
  const auto value = defaultValue.value_or("");
  text             = ui::textConfigFromColor(value, pge::colors::BLACK, ui::TextAlignment::LEFT);

  auto field = std::make_unique<ui::UiTextField>(fieldConfig, bg, text);
  auto out   = field.get();
  textFieldSection->addMenu(std::move(field));

  mainPanel.addMenu(std::move(textFieldSection));

  return out;
}
} // namespace

void CredentialsUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                           pge::sprites::TexturePack & /*texturesLoader*/)
{
  const pge::Vec2i loginDimsPixels{400, 200};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 200;
  const pge::Vec2i loginPos{(dimensions.x - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  m_credentialsPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  m_nameTextField     = addTextFieldSectionToMenu(*m_credentialsPanel, "Name:", "colo");
  m_passwordTextField = addTextFieldSectionToMenu(*m_credentialsPanel, "Password:", "aze");
}

bool CredentialsUiHandler::processUserInput(ui::UserInputData &inputData)
{
  return m_credentialsPanel->processUserInput(inputData);
}

void CredentialsUiHandler::render(pge::Renderer &engine) const
{
  m_credentialsPanel->render(engine);
}

void CredentialsUiHandler::updateUi() {}

} // namespace bsgalone::client
