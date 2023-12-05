
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
  generateProceedButton(width, height);
  generateQuitButton(width, height);
  generateFailureMenu(width, height);

  m_credentialsUiHandler.initializeMenus(width, height);
}

bool LoginScreenUiHandler::processUserInput(UserInputData &inputData)
{
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
  m_proceedButton->render(engine.getRenderer());
  m_quitButton->render(engine.getRenderer());
  m_failureMenu->render(engine.getRenderer());

  m_credentialsUiHandler.render(engine);
}

void LoginScreenUiHandler::updateUi()
{
  m_failureMenu->update();
}

void LoginScreenUiHandler::reset()
{
  m_credentialsUiHandler.reset();
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
