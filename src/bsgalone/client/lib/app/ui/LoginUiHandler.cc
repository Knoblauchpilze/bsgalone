
#include "LoginUiHandler.hh"
#include "ExitCommand.hh"
#include "LoginCommand.hh"
#include "ScreenCommon.hh"

namespace bsgalone::client {

LoginUiHandler::LoginUiHandler(IUiEventQueueShPtr eventQueue)
  : IUiHandler()
  , m_eventQueue(std::move(eventQueue))
{
  if (m_eventQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
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

void LoginUiHandler::initializeMenus(const int width,
                                     const int height,
                                     pge::sprites::TexturePack & /*texturesLoader*/)
{
  const pge::Vec2i loginDimsPixels{400, 200};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 200;
  const pge::Vec2i loginPos{(width - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  m_credentialsPanel = generateBlankVerticalMenu(loginPos, loginDimsPixels);

  m_nameTextField     = addTextFieldSectionToMenu(*m_credentialsPanel, "Name:", "colo");
  m_passwordTextField = addTextFieldSectionToMenu(*m_credentialsPanel, "Password:", "aze");

  generateRolePanel(width, height);
  generateProceedButton(width, height);
  generateQuitButton(width, height);
}

bool LoginUiHandler::processUserInput(ui::UserInputData &inputData)
{
  if (m_credentialsPanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_rolePanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_quitButton->processUserInput(inputData))
  {
    return true;
  }
  return m_proceedButton->processUserInput(inputData);
}

void LoginUiHandler::render(pge::Renderer &engine) const
{
  m_credentialsPanel->render(engine);
  m_rolePanel->render(engine);
  m_proceedButton->render(engine);
  m_quitButton->render(engine);
}

namespace {
constexpr auto GAME_ROLE_BUTTON_ACTIVE_COLOR   = almostOpaque(pge::colors::VERY_DARK_GREY);
constexpr auto GAME_ROLE_BUTTON_INACTIVE_COLOR = almostOpaque(pge::colors::DARK_GREY);
} // namespace

void LoginUiHandler::updateUi()
{
  const auto pilotButtonColor  = core::GameRole::PILOT == m_activeRole
                                   ? GAME_ROLE_BUTTON_ACTIVE_COLOR
                                   : GAME_ROLE_BUTTON_INACTIVE_COLOR;
  const auto gunnerButtonColor = core::GameRole::GUNNER == m_activeRole
                                   ? GAME_ROLE_BUTTON_ACTIVE_COLOR
                                   : GAME_ROLE_BUTTON_INACTIVE_COLOR;

  m_pilotButton->updateBgColor(pilotButtonColor);
  m_pilotButton->setHighlightable(core::GameRole::PILOT != m_activeRole);
  m_gunnerButton->updateBgColor(gunnerButtonColor);
  m_gunnerButton->setHighlightable(core::GameRole::GUNNER != m_activeRole);
}

void LoginUiHandler::generateRolePanel(const int width, const int /*height*/)
{
  constexpr auto ROLE_Y_PIXELS = 110;
  const pge::Vec2i roleDimsPixels{200, 50};
  const pge::Vec2i rolePos{(width - roleDimsPixels.x) / 2, ROLE_Y_PIXELS};

  m_rolePanel = generateBlankHorizontalMenu(rolePos, roleDimsPixels);

  ui::MenuConfig config{};

  config.clickCallback = [this]() { m_activeRole = core::GameRole::PILOT; };
  auto bg              = ui::bgConfigFromColor(GAME_ROLE_BUTTON_ACTIVE_COLOR);
  auto text            = ui::textConfigFromColor("Pilot", pge::colors::WHITE);
  auto button          = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_pilotButton        = button.get();
  m_rolePanel->addMenu(std::move(button));

  config.clickCallback = [this]() { m_activeRole = core::GameRole::GUNNER; };
  bg                   = ui::bgConfigFromColor(GAME_ROLE_BUTTON_INACTIVE_COLOR);
  text                 = ui::textConfigFromColor("Gunner", pge::colors::WHITE);
  button               = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_gunnerButton       = button.get();
  m_rolePanel->addMenu(std::move(button));
}

void LoginUiHandler::generateProceedButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;

  const pge::Vec2i loginButtonDimsPixels{200, 50};
  const pge::Vec2i loginButtonPos{(width - loginButtonDimsPixels.x) / 2,
                                  height - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const ui::MenuConfig config{
    .pos           = loginButtonPos,
    .dims          = loginButtonDimsPixels,
    .clickCallback = [this]() { triggerLoginCommand(); },
  };

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_COBALT_BLUE);
  const auto text = ui::textConfigFromColor("Login", pge::colors::WHITE);
  m_proceedButton = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

void LoginUiHandler::generateQuitButton(const int width, const int /*height*/)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const pge::Vec2i quitButtonDimsPixels{100, 50};
  const pge::Vec2i quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                 REASONABLE_GAP_SIZE};

  const ui::MenuConfig config{
    .pos           = quitButtonPos,
    .dims          = quitButtonDimsPixels,
    .clickCallback = [this]() { triggerExitCommand(); },
  };

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_GREY);
  const auto text = ui::textConfigFromColor("Quit", pge::colors::WHITE);
  m_quitButton    = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

void LoginUiHandler::triggerLoginCommand()
{
  m_eventQueue->pushEvent(std::make_unique<LoginCommand>(m_nameTextField->getText(),
                                                         m_passwordTextField->getText(),
                                                         m_activeRole));
}

void LoginUiHandler::triggerExitCommand()
{
  m_eventQueue->pushEvent(std::make_unique<ExitCommand>());
}

} // namespace bsgalone::client
