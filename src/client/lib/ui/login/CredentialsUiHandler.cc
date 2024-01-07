
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
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};

auto addTextFieldSectionToMenu(UiMenu &mainPanel,
                               const std::string &textFieldlabel,
                               const std::optional<std::string> &defaultValue = {}) -> UiTextField *
{
  auto textFieldSection = generateBlankHorizontalMenu();

  const MenuConfig config{.dims = DUMMY_DIMENSION, .highlightable = false};
  auto bg    = bgConfigFromColor(semiOpaque(olc::DARK_BLUE));
  auto text  = textConfigFromColor(textFieldlabel, olc::GREY, TextAlignment::RIGHT);
  auto label = std::make_unique<UiTextMenu>(config, bg, text);
  textFieldSection->addMenu(std::move(label));

  TextFieldConfig fieldConfig{.dims = DUMMY_DIMENSION};
  bg               = bgConfigFromColor(semiOpaque(olc::WHITE));
  const auto value = defaultValue.value_or("");
  text             = textConfigFromColor(value, olc::BLACK, TextAlignment::LEFT);

  auto field = std::make_unique<UiTextField>(fieldConfig, bg, text);
  auto out   = field.get();
  textFieldSection->addMenu(std::move(field));

  mainPanel.addMenu(std::move(textFieldSection));

  return out;
}
} // namespace

void CredentialsUiHandler::initializeMenus(const int width, const int /*height*/)
{
  const olc::vi2d loginDimsPixels{400, 200};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 200;
  const olc::vi2d loginPos{(width - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  MenuConfig config{.pos = loginPos, .dims = loginDimsPixels, .highlightable = false};
  m_credentialsPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  m_nameTextField     = addTextFieldSectionToMenu(*m_credentialsPanel, "Name:", "grouton");
  m_passwordTextField = addTextFieldSectionToMenu(*m_credentialsPanel, "Password:");
}

bool CredentialsUiHandler::processUserInput(UserInputData &inputData)
{
  return m_credentialsPanel->processUserInput(inputData);
}

void CredentialsUiHandler::render(SpriteRenderer &engine) const
{
  m_credentialsPanel->render(engine.getRenderer());
}

void CredentialsUiHandler::updateUi() {}

} // namespace pge
