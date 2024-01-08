
#include "LoginScreenUiHandler.hh"
#include "LoginMessage.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenUiHandler::LoginScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("login")
  , AbstractMessageListener({bsgo::MessageType::LOGIN})
  , m_playerView(views.playerView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void LoginScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateLoginModePanel(width, height);
  generateFactionPanel(width, height);
  generateProceedButton(width, height);
  generateQuitButton(width, height);
  generateFailureMenu(width, height);

  m_credentialsUiHandler.initializeMenus(width, height);
}

bool LoginScreenUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_loginModePanel->processUserInput(inputData))
  {
    return true;
  }
  if (m_factionPanel->processUserInput(inputData))
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

void LoginScreenUiHandler::render(Renderer &engine) const
{
  m_loginModePanel->render(engine);
  m_factionPanel->render(engine);
  m_proceedButton->render(engine);
  m_quitButton->render(engine);
  m_failureMenu->render(engine);

  m_credentialsUiHandler.render(engine);
}

namespace {
const auto LOGIN_BUTTON_ACTIVE_COLOR   = semiOpaque(olc::DARK_GREEN);
const auto LOGIN_BUTTON_INACTIVE_COLOR = semiOpaque(olc::DARK_BLUE);

constexpr auto LOGIN_TEXT  = "AU JEU!!";
constexpr auto SIGNUP_TEXT = "Sign up";

constexpr auto LOGIN_FAILURE_TEXT  = "Login failed, check your credentials!";
constexpr auto SIGNUP_FAILURE_TEXT = "Sign up failed, check your credentials!";

const auto COLONIAL_BUTTON_ACTIVE_COLOR   = almostOpaque(olc::VERY_DARK_BLUE);
const auto COLONIAL_BUTTON_INACTIVE_COLOR = almostOpaque(olc::DARK_BLUE);

const auto CYLON_BUTTON_ACTIVE_COLOR   = almostOpaque(olc::VERY_DARK_RED);
const auto CYLON_BUTTON_INACTIVE_COLOR = almostOpaque(olc::DARK_RED);
} // namespace

void LoginScreenUiHandler::updateUi()
{
  const auto loginButtonColor  = Mode::LOGIN == m_mode ? LOGIN_BUTTON_ACTIVE_COLOR
                                                       : LOGIN_BUTTON_INACTIVE_COLOR;
  const auto signupButtonColor = Mode::SIGNUP == m_mode ? LOGIN_BUTTON_ACTIVE_COLOR
                                                        : LOGIN_BUTTON_INACTIVE_COLOR;

  m_loginButton->updateBgColor(loginButtonColor);
  m_loginButton->setHighlightable(Mode::LOGIN != m_mode);
  m_signupButton->updateBgColor(signupButtonColor);
  m_signupButton->setHighlightable(Mode::SIGNUP != m_mode);

  m_factionPanel->setVisible(Mode::SIGNUP == m_mode);
  if (Mode::SIGNUP == m_mode)
  {
    const auto colonialButtonColor = bsgo::Faction::COLONIAL == m_faction
                                       ? COLONIAL_BUTTON_ACTIVE_COLOR
                                       : COLONIAL_BUTTON_INACTIVE_COLOR;
    const auto cylonButtonColor    = bsgo::Faction::CYLON == m_faction ? CYLON_BUTTON_ACTIVE_COLOR
                                                                       : CYLON_BUTTON_INACTIVE_COLOR;

    m_colonialButton->updateBgColor(colonialButtonColor);
    m_colonialButton->setHighlightable(bsgo::Faction::COLONIAL != m_faction);
    m_cylonButton->updateBgColor(cylonButtonColor);
    m_cylonButton->setHighlightable(bsgo::Faction::CYLON != m_faction);
  }

  const auto text = Mode::LOGIN == m_mode ? LOGIN_TEXT : SIGNUP_TEXT;
  m_proceedButton->setText(text);

  const auto failureText = Mode::LOGIN == m_mode ? LOGIN_FAILURE_TEXT : SIGNUP_FAILURE_TEXT;
  m_failureMenuText->setText(failureText);

  m_failureMenu->update();
}

void LoginScreenUiHandler::reset()
{
  m_credentialsUiHandler.reset();
}

void LoginScreenUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  m_credentialsUiHandler.connectToMessageQueue(messageQueue);

  messageQueue.addListener(this);
}

void LoginScreenUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  const auto &loginMessage = message.as<bsgo::LoginMessage>();
  if (bsgo::LoginState::REJECTED == loginMessage.getLoginState())
  {
    m_failureMenu->trigger();
  }
}

void LoginScreenUiHandler::generateLoginModePanel(const int width, const int /*height*/)
{
  constexpr auto LOGIN_MODE_Y_PIXELS = 20;
  const Vec2i loginModeDimsPixels{200, 50};
  const Vec2i loginModePos{(width - loginModeDimsPixels.x) / 2, LOGIN_MODE_Y_PIXELS};

  m_loginModePanel = generateBlankHorizontalMenu(loginModePos, loginModeDimsPixels);

  MenuConfig config{};
  const auto bg = bgConfigFromColor(LOGIN_BUTTON_ACTIVE_COLOR);

  config.clickCallback = [this]() { setLoginMode(Mode::LOGIN); };
  auto text            = textConfigFromColor("Login", colors::WHITE);
  auto button          = std::make_unique<UiTextMenu>(config, bg, text);
  m_loginButton        = button.get();
  m_loginModePanel->addMenu(std::move(button));

  config.clickCallback = [this]() { setLoginMode(Mode::SIGNUP); };
  text                 = textConfigFromColor("Sign up", colors::WHITE);
  button               = std::make_unique<UiTextMenu>(config, bg, text);
  m_signupButton       = button.get();
  m_loginModePanel->addMenu(std::move(button));
}

void LoginScreenUiHandler::generateFactionPanel(const int width, const int /*height*/)
{
  constexpr auto FACTION_Y_PIXELS = 110;
  const Vec2i factionDimsPixels{200, 50};
  const Vec2i factionPos{(width - factionDimsPixels.x) / 2, FACTION_Y_PIXELS};

  m_factionPanel = generateBlankHorizontalMenu(factionPos, factionDimsPixels);

  MenuConfig config{};

  config.clickCallback = [this]() { setFaction(bsgo::Faction::COLONIAL); };
  auto bg              = bgConfigFromColor(COLONIAL_BUTTON_ACTIVE_COLOR);
  auto text            = textConfigFromColor("Colonial", colors::WHITE);
  auto button          = std::make_unique<UiTextMenu>(config, bg, text);
  m_colonialButton     = button.get();
  m_factionPanel->addMenu(std::move(button));

  config.clickCallback = [this]() { setFaction(bsgo::Faction::CYLON); };
  bg                   = bgConfigFromColor(CYLON_BUTTON_INACTIVE_COLOR);
  text                 = textConfigFromColor("Cylon", colors::WHITE);
  button               = std::make_unique<UiTextMenu>(config, bg, text);
  m_cylonButton        = button.get();
  m_factionPanel->addMenu(std::move(button));
}

void LoginScreenUiHandler::generateProceedButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;

  const Vec2i loginButtonDimsPixels{200, 50};
  const Vec2i loginButtonPos{(width - loginButtonDimsPixels.x) / 2,
                             height - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const MenuConfig config{.pos           = loginButtonPos,
                          .dims          = loginButtonDimsPixels,
                          .clickCallback = [this]() { tryLogin(); }};

  const auto bg   = bgConfigFromColor(colors::toOlcPixel(colors::Name::DARK_COBALT_BLUE));
  const auto text = textConfigFromColor(LOGIN_TEXT, colors::WHITE);
  m_proceedButton = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateQuitButton(const int width, const int /*height*/)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const Vec2i quitButtonDimsPixels{100, 50};
  const Vec2i quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                            REASONABLE_GAP_SIZE};

  const MenuConfig config{.pos               = quitButtonPos,
                          .dims              = quitButtonDimsPixels,
                          .gameClickCallback = [](Game &g) {
                            g.setScreen(Screen::EXIT);
                            g.terminate();
                          }};

  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Quit", colors::WHITE);
  m_quitButton    = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateFailureMenu(const int width, const int /*height*/)
{
  const Vec2i failureMenuDimsPixels{350, 80};
  const Vec2i failureMenuPos{(width - failureMenuDimsPixels.x) / 2, 430};

  const MenuConfig config{.pos           = failureMenuPos,
                          .dims          = failureMenuDimsPixels,
                          .highlightable = false};

  const auto bg   = bgConfigFromColor(olc::DARK_RED);
  const auto text = textConfigFromColor(LOGIN_FAILURE_TEXT, colors::BLACK);

  auto menu         = std::make_unique<UiTextMenu>(config, bg, text);
  m_failureMenuText = menu.get();
  m_failureMenu     = std::make_unique<UiTimedMenu>(std::move(menu));
}

void LoginScreenUiHandler::setLoginMode(const Mode &mode)
{
  m_mode = mode;
}

void LoginScreenUiHandler::setFaction(const bsgo::Faction &faction)
{
  m_faction = faction;
}

void LoginScreenUiHandler::tryLogin()
{
  const auto data = m_credentialsUiHandler.getCredentials();

  if (data.name.empty() || data.password.empty())
  {
    warn("Can't login with empty name or password");
    m_failureMenu->trigger();
    return;
  }

  switch (m_mode)
  {
    case Mode::LOGIN:
      m_playerView->tryLogin(data.name, data.password);
      break;
    case Mode::SIGNUP:
      m_playerView->trySignup(data.name, data.password, m_faction);
      break;
    default:
      error("Unknown mode");
      break;
  }
}

} // namespace pge
