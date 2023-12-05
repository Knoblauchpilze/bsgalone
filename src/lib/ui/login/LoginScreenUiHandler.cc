
#include "LoginScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenUiHandler::LoginScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("login")
  , m_loginView(views.loginView)
{
  if (nullptr == m_loginView)
  {
    throw std::invalid_argument("Expected non null login view");
  }
}

void LoginScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateLoginModePanel(width, height);
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

void LoginScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_loginModePanel->render(engine.getRenderer());
  m_proceedButton->render(engine.getRenderer());
  m_quitButton->render(engine.getRenderer());
  m_failureMenu->render(engine.getRenderer());

  m_credentialsUiHandler.render(engine);
}

namespace {
const auto LOGIN_BUTTON_ACTIVE_COLOR   = transparent(olc::DARK_GREEN, alpha::SemiOpaque);
const auto LOGIN_BUTTON_INACTIVE_COLOR = transparent(olc::DARK_BLUE, alpha::SemiOpaque);
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

  m_failureMenu->update();
}

void LoginScreenUiHandler::reset()
{
  m_credentialsUiHandler.reset();
}

namespace {
constexpr auto DUMMY_PIXEL_DIMS = 10;
const auto DUMMY_DIMENSIONS     = olc::vi2d{DUMMY_PIXEL_DIMS, DUMMY_PIXEL_DIMS};
} // namespace

void LoginScreenUiHandler::generateLoginModePanel(const int width, const int /*height*/)
{
  constexpr auto LOGIN_MODE_Y_PIXELS = 50;
  const olc::vi2d loginModeDimsPixels{200, 80};
  const olc::vi2d loginModePos{(width - loginModeDimsPixels.x) / 2, LOGIN_MODE_Y_PIXELS};

  m_loginModePanel = generateBlankHorizontalMenu(loginModePos, loginModeDimsPixels);

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSIONS};
  const auto bg = bgConfigFromColor(LOGIN_BUTTON_ACTIVE_COLOR);

  auto text     = textConfigFromColor("Login", olc::WHITE);
  auto button   = std::make_unique<UiTextMenu>(config, bg, text);
  m_loginButton = button.get();
  m_loginModePanel->addMenu(std::move(button));

  text           = textConfigFromColor("Sign up", olc::WHITE);
  button         = std::make_unique<UiTextMenu>(config, bg, text);
  m_signupButton = button.get();
  m_loginModePanel->addMenu(std::move(button));
}

void LoginScreenUiHandler::generateProceedButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 60;

  const olc::vi2d loginButtonDimsPixels{200, 60};
  const olc::vi2d loginButtonPos{(width - loginButtonDimsPixels.x) / 2,
                                 height - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const MenuConfig config{.pos               = loginButtonPos,
                          .dims              = loginButtonDimsPixels,
                          .gameClickCallback = [this](Game &g) { tryLogin(g); }};

  const auto bg   = bgConfigFromColor(olc::DARK_COBALT_BLUE);
  const auto text = textConfigFromColor("AU JEU!!", olc::WHITE);
  m_proceedButton = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateQuitButton(const int width, const int /*height*/)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const olc::vi2d quitButtonDimsPixels{100, 50};
  const olc::vi2d quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                REASONABLE_GAP_SIZE};

  const MenuConfig config{.pos               = quitButtonPos,
                          .dims              = quitButtonDimsPixels,
                          .gameClickCallback = [](Game &g) {
                            g.setScreen(Screen::EXIT);
                            g.terminate();
                          }};

  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Quit", olc::WHITE);
  m_quitButton    = std::make_unique<UiTextMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateFailureMenu(const int width, const int height)
{
  const olc::vi2d failureMenuDimsPixels{300, 150};
  const olc::vi2d failureMenuPos{(width - failureMenuDimsPixels.x) / 2,
                                 (height - failureMenuDimsPixels.y) / 2};

  const MenuConfig config{.pos           = failureMenuPos,
                          .dims          = failureMenuDimsPixels,
                          .highlightable = false};

  const auto bg   = bgConfigFromColor(olc::DARK_RED);
  const auto text = textConfigFromColor("Login failed, check your credentials!", olc::BLACK);

  auto menu     = std::make_unique<UiTextMenu>(config, bg, text);
  m_failureMenu = std::make_unique<UiTimedMenu>(std::move(menu));
}

void LoginScreenUiHandler::tryLogin(Game &game)
{
  const auto data = m_credentialsUiHandler.getCredentials();

  if (data.name.empty() || data.password.empty())
  {
    warn("Can't login with empty name or password");
    m_failureMenu->trigger();
    return;
  }

  const auto playerId = m_loginView->tryLogin(data.name, data.password);
  if (playerId)
  {
    game.login(*playerId);
  }
  else
  {
    m_failureMenu->trigger();
  }
}

} // namespace pge
