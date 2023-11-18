
#include "LoginScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenUiHandler::LoginScreenUiHandler()
  : IUiHandler("login")
{}

void LoginScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateLoginMenu(width, height);
  generateLoginButton(width, height);
  generateQuitMenu(width, height);
}

bool LoginScreenUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_quitButton->processUserInput(inputData))
  {
    return true;
  }
  if (m_loginButton->processUserInput(inputData))
  {
    return true;
  }

  return m_loginPanel->processUserInput(inputData);
}

void LoginScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_loginPanel->render(engine.getRenderer());
  m_loginButton->render(engine.getRenderer());
  m_quitButton->render(engine.getRenderer());
}

void LoginScreenUiHandler::updateUi() {}

void LoginScreenUiHandler::generateLoginMenu(const int width, const int /*height*/)
{
  const olc::vi2d loginDimsPixels{400, 400};
  constexpr auto LOGIN_PANEL_Y_PIXELS = 30;
  const olc::vi2d loginPos{(width - loginDimsPixels.x) / 2, LOGIN_PANEL_Y_PIXELS};

  MenuConfig config{.pos = loginPos, .dims = loginDimsPixels, .highligtable = false};
  auto bg      = bgConfigFromColor(transparent(olc::WHITE));
  m_loginPanel = std::make_unique<UiMenu>(config, bg);

  bg         = bgConfigFromColor(transparent(olc::DARK_BLUE, alpha::SemiOpaque));
  auto text  = textConfigFromColor("Name:", olc::DARK_GREY, TextAlignment::LEFT);
  auto label = std::make_unique<UiMenu>(config, bg, text);
  m_loginPanel->addMenu(std::move(label));

  bg    = bgConfigFromColor(transparent(olc::WHITE, alpha::SemiOpaque));
  text  = textConfigFromColor("haha", olc::BLACK, TextAlignment::LEFT);
  label = std::make_unique<UiMenu>(config, bg, text);
  m_loginPanel->addMenu(std::move(label));

  bg    = bgConfigFromColor(transparent(olc::DARK_BLUE, alpha::SemiOpaque));
  text  = textConfigFromColor("Password:", olc::DARK_GREY, TextAlignment::LEFT);
  label = std::make_unique<UiMenu>(config, bg, text);
  m_loginPanel->addMenu(std::move(label));

  bg    = bgConfigFromColor(transparent(olc::WHITE, alpha::SemiOpaque));
  text  = textConfigFromColor("pwd", olc::BLACK, TextAlignment::LEFT);
  label = std::make_unique<UiMenu>(config, bg, text);
  m_loginPanel->addMenu(std::move(label));
}

void LoginScreenUiHandler::generateLoginButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 60;

  const olc::vi2d loginButtonDimsPixels{200, 60};
  const olc::vi2d loginButtonPos{(width - loginButtonDimsPixels.x) / 2,
                                 height - REASONABLE_GAP_SIZE - loginButtonDimsPixels.y};

  const MenuConfig config{.pos = loginButtonPos, .dims = loginButtonDimsPixels};

  const auto bg   = bgConfigFromColor(olc::DARK_COBALT_BLUE);
  const auto text = textConfigFromColor("AU JEU!!", olc::WHITE);
  m_loginButton   = std::make_unique<UiMenu>(config, bg, text);
}

void LoginScreenUiHandler::generateQuitMenu(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const olc::vi2d quitButtonDimsPixels{100, 50};
  const olc::vi2d quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                height - REASONABLE_GAP_SIZE - quitButtonDimsPixels.y};

  const MenuConfig config{.pos               = quitButtonPos,
                          .dims              = quitButtonDimsPixels,
                          .gameClickCallBack = [](Game &g) {
                            g.setScreen(Screen::EXIT);
                            g.terminate();
                          }};

  const auto bg   = bgConfigFromColor(olc::VERY_DARK_GREY);
  const auto text = textConfigFromColor("Quit", olc::WHITE);
  m_quitButton    = std::make_unique<UiMenu>(config, bg, text);
}

} // namespace pge
