
#include "CredentialsUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

CredentialsUiHandler::CredentialsUiHandler()
  : IUiHandler("credentials")
{}

auto CredentialsUiHandler::getCredentials() const -> PlayerCredentials
{
  return PlayerCredentials{.name     = m_nameTextField->getText(),
                           .password = m_passwordTextField->getText()};
}

namespace {
auto addTextFieldSectionToMenu(UiMenu &mainPanel,
                               const std::string &textFieldlabel,
                               const std::optional<std::string> &defaultValue = {}) -> UiTextField *
{
  auto textFieldSection = generateBlankHorizontalMenu();

  const MenuConfig config{.highlightable = false};
  auto bg    = bgConfigFromColor(semiOpaque(colors::DARK_BLUE));
  auto text  = textConfigFromColor(textFieldlabel, colors::GREY, TextAlignment::RIGHT);
  auto label = std::make_unique<UiTextMenu>(config, bg, text);
  textFieldSection->addMenu(std::move(label));

  TextFieldConfig fieldConfig{};
  bg               = bgConfigFromColor(semiOpaque(colors::WHITE));
  const auto value = defaultValue.value_or("");
  text             = textConfigFromColor(value, colors::BLACK, TextAlignment::LEFT);

  auto field = std::make_unique<UiTextField>(fieldConfig, bg, text);
  auto out   = field.get();
  textFieldSection->addMenu(std::move(field));

  mainPanel.addMenu(std::move(textFieldSection));

  return out;
}
} // namespace

void CredentialsUiHandler::initializeMenus(const int width, const int /*height*/)
{
  const Vec2i loginDimsPixels{400, 200};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 200;
  const Vec2i loginPos{(width - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  m_credentialsPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  m_nameTextField     = addTextFieldSectionToMenu(*m_credentialsPanel, "Name:", "grouton");
  m_passwordTextField = addTextFieldSectionToMenu(*m_credentialsPanel, "Password:");
}

bool CredentialsUiHandler::processUserInput(UserInputData &inputData)
{
  return m_credentialsPanel->processUserInput(inputData);
}

void CredentialsUiHandler::render(Renderer &engine) const
{
  m_credentialsPanel->render(engine);
}

void CredentialsUiHandler::updateUi() {}

} // namespace pge
