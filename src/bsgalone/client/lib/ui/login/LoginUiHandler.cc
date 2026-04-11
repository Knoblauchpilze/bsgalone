
#include "LoginUiHandler.hh"
#include "ExitCommand.hh"
#include "IUiEventListener.hh"
#include "LoginCommand.hh"
#include "LoginFailedEvent.hh"
#include "Palette.hh"
#include "ScreenCommon.hh"
#include "SignupCommand.hh"

namespace bsgalone::client {
namespace {
const std::unordered_set<UiEventType> RELEVANT_EVENT_TYPES = {
  UiEventType::LOGIN_FAILED,
  UiEventType::SIGNUP_FAILED,
  UiEventType::SIGNUP_SUCCEEDED,
};
}

class UiEventListenerLoginProxy : public IUiEventListener
{
  public:
  explicit UiEventListenerLoginProxy(LoginUiHandler &handler)
    : IUiEventListener()
    , m_handler(handler)
  {}

  ~UiEventListenerLoginProxy() override = default;

  bool isEventRelevant(const UiEventType &type) const override
  {
    return RELEVANT_EVENT_TYPES.contains(type);
  }

  void onEventReceived(const IUiEvent &event) override
  {
    switch (event.type())
    {
      case UiEventType::LOGIN_FAILED:
        m_handler.onLoginFailed();
        break;
      case UiEventType::SIGNUP_FAILED:
        m_handler.onSignupFailed();
        break;
      case UiEventType::SIGNUP_SUCCEEDED:
        m_handler.onSignupSucceeded();
        break;
      default:
        throw std::invalid_argument("Unsupported UI event type " + str(event.type()));
    }
  }

  private:
  LoginUiHandler &m_handler;
};

LoginUiHandler::LoginUiHandler(IUiEventQueueShPtr inputQueue, IUiCommandQueueShPtr outputQueue)
  : IUiHandler()
  , ::core::CoreObject("login")
  , m_queue(std::move(outputQueue))
{
  if (inputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null command queue");
  }

  registerToQueue(std::move(inputQueue));
}

void LoginUiHandler::initializeMenus(const pge::Vec2i &dimensions,
                                     pge::sprites::TexturePack &texturesLoader)
{
  generateLoginModePanel(dimensions);
  generateFactionPanel(dimensions);
  generateRolePanel(dimensions);
  generateProceedButton(dimensions);
  generateQuitButton(dimensions);
  generateFailureMenu(dimensions);
  generateSuccessfulSignupMenu(dimensions);

  m_credentialsUiHandler.initializeMenus(dimensions, texturesLoader);
}

bool LoginUiHandler::processUserInput(ui::UserInputData &inputData)
{
  if (m_loginModePanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_factionPanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_rolePanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_credentialsUiHandler.processUserInput(inputData))
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
  m_loginModePanel->render(engine);
  m_factionPanel->render(engine);
  m_rolePanel->render(engine);
  m_proceedButton->render(engine);
  m_quitButton->render(engine);
  m_failureMenu->render(engine);
  m_successfulSignupMenu->render(engine);

  m_credentialsUiHandler.render(engine);
}

namespace {
constexpr auto LOGIN_BUTTON_ACTIVE_COLOR   = semiOpaque(pge::colors::DARK_GREEN);
constexpr auto LOGIN_BUTTON_INACTIVE_COLOR = semiOpaque(pge::colors::DARK_BLUE);

constexpr auto LOGIN_TEXT  = "AU JEU!!";
constexpr auto SIGNUP_TEXT = "Sign up";

constexpr auto LOGIN_FAILURE_TEXT  = "Login failed, check your credentials!";
constexpr auto SIGNUP_FAILURE_TEXT = "Sign up failed, check your credentials!";
constexpr auto SIGNUP_SUCCESS_TEXT = "Signup succeeded, login to start playing!";

constexpr auto GAME_ROLE_BUTTON_ACTIVE_COLOR   = almostOpaque(pge::colors::VERY_DARK_GREY);
constexpr auto GAME_ROLE_BUTTON_INACTIVE_COLOR = almostOpaque(pge::colors::DARK_GREY);
} // namespace

void LoginUiHandler::updateUi()
{
  const auto loginButtonColor  = Mode::LOGIN == m_mode ? LOGIN_BUTTON_ACTIVE_COLOR
                                                       : LOGIN_BUTTON_INACTIVE_COLOR;
  const auto signupButtonColor = Mode::SIGNUP == m_mode ? LOGIN_BUTTON_ACTIVE_COLOR
                                                        : LOGIN_BUTTON_INACTIVE_COLOR;

  const auto cylonPalette    = generatePaletteForFaction(core::Faction::CYLON);
  const auto colonialPalette = generatePaletteForFaction(core::Faction::COLONIAL);

  m_loginButton->updateBgColor(loginButtonColor);
  m_loginButton->setHighlightable(Mode::LOGIN != m_mode);
  m_signupButton->updateBgColor(signupButtonColor);
  m_signupButton->setHighlightable(Mode::SIGNUP != m_mode);

  m_factionPanel->setVisible(Mode::SIGNUP == m_mode);
  if (Mode::SIGNUP == m_mode)
  {
    const auto colonialButtonColor = core::Faction::COLONIAL == m_faction
                                       ? colonialPalette.activeButtonColor
                                       : cylonPalette.inactiveButtonColor;
    const auto cylonButtonColor    = core::Faction::CYLON == m_faction
                                       ? cylonPalette.activeButtonColor
                                       : colonialPalette.inactiveButtonColor;

    m_colonialButton->updateBgColor(colonialButtonColor);
    m_colonialButton->setHighlightable(core::Faction::COLONIAL != m_faction);
    m_cylonButton->updateBgColor(cylonButtonColor);
    m_cylonButton->setHighlightable(core::Faction::CYLON != m_faction);
  }

  m_rolePanel->setVisible(Mode::LOGIN == m_mode);
  if (Mode::LOGIN == m_mode)
  {
    const auto pilotButtonColor  = core::GameRole::PILOT == m_role ? GAME_ROLE_BUTTON_ACTIVE_COLOR
                                                                   : GAME_ROLE_BUTTON_INACTIVE_COLOR;
    const auto gunnerButtonColor = core::GameRole::GUNNER == m_role
                                     ? GAME_ROLE_BUTTON_ACTIVE_COLOR
                                     : GAME_ROLE_BUTTON_INACTIVE_COLOR;

    m_pilotButton->updateBgColor(pilotButtonColor);
    m_pilotButton->setHighlightable(core::GameRole::PILOT != m_role);
    m_gunnerButton->updateBgColor(gunnerButtonColor);
    m_gunnerButton->setHighlightable(core::GameRole::GUNNER != m_role);
  }

  const auto text = Mode::LOGIN == m_mode ? LOGIN_TEXT : SIGNUP_TEXT;
  m_proceedButton->setText(text);

  const auto failureText = Mode::LOGIN == m_mode ? LOGIN_FAILURE_TEXT : SIGNUP_FAILURE_TEXT;
  m_failureMenuText->setText(failureText);

  m_failureMenu->update();
  m_successfulSignupMenu->update();
}

void LoginUiHandler::registerToQueue(IUiEventQueueShPtr inputQueue)
{
  inputQueue->addListener(std::make_unique<UiEventListenerLoginProxy>(*this));
}

void LoginUiHandler::generateLoginModePanel(const pge::Vec2i &dimensions)
{
  constexpr auto LOGIN_MODE_Y_PIXELS = 20;
  const pge::Vec2i loginModeDimsPixels{200, 50};
  const pge::Vec2i loginModePos{(dimensions.x - loginModeDimsPixels.x) / 2, LOGIN_MODE_Y_PIXELS};

  m_loginModePanel = generateBlankHorizontalMenu(loginModePos, loginModeDimsPixels);

  ui::MenuConfig config{};
  const auto bg = ui::bgConfigFromColor(LOGIN_BUTTON_ACTIVE_COLOR);

  config.clickCallback = [this]() { setLoginMode(Mode::LOGIN); };
  auto text            = ui::textConfigFromColor("Login", pge::colors::WHITE);
  auto button          = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_loginButton        = button.get();
  m_loginModePanel->addMenu(std::move(button));

  config.clickCallback = [this]() { setLoginMode(Mode::SIGNUP); };
  text                 = ui::textConfigFromColor("Sign up", pge::colors::WHITE);
  button               = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_signupButton       = button.get();
  m_loginModePanel->addMenu(std::move(button));
}

void LoginUiHandler::generateFactionPanel(const pge::Vec2i &dimensions)
{
  constexpr auto FACTION_Y_PIXELS = 110;
  const pge::Vec2i factionDimsPixels{200, 50};
  const pge::Vec2i factionPos{(dimensions.x - factionDimsPixels.x) / 2, FACTION_Y_PIXELS};

  m_factionPanel = generateBlankHorizontalMenu(factionPos, factionDimsPixels);

  ui::MenuConfig config{};

  const auto colonialPalette = generatePaletteForFaction(core::Faction::COLONIAL);
  const auto cylonPalette    = generatePaletteForFaction(core::Faction::CYLON);

  config.clickCallback = [this]() { setFaction(core::Faction::COLONIAL); };
  auto bg              = ui::bgConfigFromColor(colonialPalette.activeButtonColor);
  auto text            = ui::textConfigFromColor("Colonial", pge::colors::WHITE);
  auto button          = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_colonialButton     = button.get();
  m_factionPanel->addMenu(std::move(button));

  config.clickCallback = [this]() { setFaction(core::Faction::CYLON); };
  bg                   = ui::bgConfigFromColor(cylonPalette.inactiveButtonColor);
  text                 = ui::textConfigFromColor("Cylon", pge::colors::WHITE);
  button               = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_cylonButton        = button.get();
  m_factionPanel->addMenu(std::move(button));
}

void LoginUiHandler::generateRolePanel(const pge::Vec2i &dimensions)
{
  constexpr auto ROLE_Y_PIXELS = 110;
  const pge::Vec2i roleDimsPixels{200, 50};
  const pge::Vec2i rolePos{(dimensions.x - roleDimsPixels.x) / 2, ROLE_Y_PIXELS};

  m_rolePanel = generateBlankHorizontalMenu(rolePos, roleDimsPixels);

  ui::MenuConfig config{};

  config.clickCallback = [this]() { setGameRole(core::GameRole::PILOT); };
  auto bg              = ui::bgConfigFromColor(GAME_ROLE_BUTTON_ACTIVE_COLOR);
  auto text            = ui::textConfigFromColor("Pilot", pge::colors::WHITE);
  auto button          = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_pilotButton        = button.get();
  m_rolePanel->addMenu(std::move(button));

  config.clickCallback = [this]() { setGameRole(core::GameRole::GUNNER); };
  bg                   = ui::bgConfigFromColor(GAME_ROLE_BUTTON_INACTIVE_COLOR);
  text                 = ui::textConfigFromColor("Gunner", pge::colors::WHITE);
  button               = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_gunnerButton       = button.get();
  m_rolePanel->addMenu(std::move(button));
}

void LoginUiHandler::generateProceedButton(const pge::Vec2i &dimensions)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;

  const pge::Vec2i loginButtonDimsPixels{200, 50};
  const pge::Vec2i loginButtonPos{(dimensions.x - loginButtonDimsPixels.x) / 2,
                                  dimensions.y - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const ui::MenuConfig config{.pos           = loginButtonPos,
                              .dims          = loginButtonDimsPixels,
                              .clickCallback = [this]() { onProceedRequested(); }};

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_COBALT_BLUE);
  const auto text = ui::textConfigFromColor(LOGIN_TEXT, pge::colors::WHITE);
  m_proceedButton = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

void LoginUiHandler::generateQuitButton(const pge::Vec2i &dimensions)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const pge::Vec2i quitButtonDimsPixels{100, 50};
  const pge::Vec2i quitButtonPos{dimensions.x - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                 REASONABLE_GAP_SIZE};

  const ui::MenuConfig config{.pos           = quitButtonPos,
                              .dims          = quitButtonDimsPixels,
                              .clickCallback = [this]() { onExitRequested(); }};

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_GREY);
  const auto text = ui::textConfigFromColor("Quit", pge::colors::WHITE);
  m_quitButton    = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

void LoginUiHandler::generateFailureMenu(const pge::Vec2i &dimensions)
{
  const pge::Vec2i failureMenuDimsPixels{350, 80};
  const pge::Vec2i failureMenuPos{(dimensions.x - failureMenuDimsPixels.x) / 2, 430};

  const ui::MenuConfig config{.pos           = failureMenuPos,
                              .dims          = failureMenuDimsPixels,
                              .highlightable = false};

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_RED);
  const auto text = ui::textConfigFromColor(LOGIN_FAILURE_TEXT, pge::colors::BLACK);

  auto menu         = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_failureMenuText = menu.get();
  m_failureMenu     = std::make_unique<ui::UiTimedMenu>(std::move(menu));
}

void LoginUiHandler::generateSuccessfulSignupMenu(const pge::Vec2i &dimensions)
{
  const pge::Vec2i signupSuccessMenuDimsPixels{350, 80};
  const pge::Vec2i signupSuccessMenuPos{(dimensions.x - signupSuccessMenuDimsPixels.x) / 2, 430};

  const ui::MenuConfig config{.pos           = signupSuccessMenuPos,
                              .dims          = signupSuccessMenuDimsPixels,
                              .highlightable = false};

  const auto bg   = ui::bgConfigFromColor(pge::colors::DARK_GREEN);
  const auto text = ui::textConfigFromColor(SIGNUP_SUCCESS_TEXT, pge::colors::BLACK);

  auto menu              = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_successfulSignupMenu = std::make_unique<ui::UiTimedMenu>(std::move(menu));
}

void LoginUiHandler::setLoginMode(const Mode mode)
{
  m_mode = mode;
}

void LoginUiHandler::setFaction(const core::Faction faction)
{
  m_faction = faction;
}

void LoginUiHandler::setGameRole(const core::GameRole role)
{
  m_role = role;
}

void LoginUiHandler::onProceedRequested()
{
  const auto data = m_credentialsUiHandler.getCredentials();

  if (!data.valid())
  {
    warn("Can't login with empty name or password");
    m_failureMenu->trigger();
    return;
  }

  switch (m_mode)
  {
    case Mode::LOGIN:
      m_queue->pushEvent(std::make_unique<LoginCommand>(data.username, data.password, m_role));
      break;
    case Mode::SIGNUP:
      m_queue->pushEvent(std::make_unique<SignupCommand>(data.username, data.password, m_faction));
      break;
    default:
      error("Unknown mode");
      break;
  }
}

void LoginUiHandler::onExitRequested()
{
  m_queue->pushEvent(std::make_unique<ExitCommand>());
}

void LoginUiHandler::onLoginFailed()
{
  m_failureMenu->trigger();
}

void LoginUiHandler::onSignupSucceeded()
{
  setLoginMode(Mode::LOGIN);
  m_successfulSignupMenu->trigger();
}

void LoginUiHandler::onSignupFailed()
{
  m_failureMenu->trigger();
}

} // namespace bsgalone::client
