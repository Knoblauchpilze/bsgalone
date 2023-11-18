
#include "LoginScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenUiHandler::LoginScreenUiHandler()
  : IUiHandler("login")
{}

void LoginScreenUiHandler::initializeMenus(const int width, const int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_PINK);

  MenuShPtr m = generateScreenOption(dims, "Login", olc::VERY_DARK_PINK, "login", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::OUTPOST); });
  m_menu->addMenu(m);

  m = generateScreenOption(dims, "Quit", olc::VERY_DARK_PINK, "quit", true);
  m->setSimpleAction([this](Game &g) {
    g.setScreen(Screen::EXIT);
    g.terminate();
  });
  m_menu->addMenu(m);

  MenuConfig config{.pos    = olc::vi2d{30, 50},
                    .dims   = olc::vi2d{150, 90},
                    .layout = MenuLayout::Horizontal};
  auto bgConfig = bgConfigFromColor(olc::Pixel{255, 255, 128});
  m_uiMenu      = std::make_unique<UiMenu>(config, bgConfig);

  // Horizontal
  config.layout     = MenuLayout::Vertical;
  bgConfig          = bgConfigFromColor(olc::DARK_BROWN);
  auto menuVertical = std::make_unique<UiMenu>(config, bgConfig);

  bgConfig        = bgConfigFromColor(olc::DARK_CYAN);
  auto textConfig = textConfigFromColor("menu1", olc::DARK_COBALT_BLUE, TextAlignment::LEFT);
  auto menu       = std::make_unique<UiMenu>(config, bgConfig, textConfig);
  menuVertical->addMenu(std::move(menu));

  bgConfig   = bgConfigFromColor(olc::DARK_ORANGE);
  textConfig = textConfigFromColor("menu2", olc::DARK_GREEN, TextAlignment::CENTER);
  menu       = std::make_unique<UiMenu>(config, bgConfig, textConfig);
  menuVertical->addMenu(std::move(menu));

  bgConfig   = bgConfigFromColor(olc::DARK_PINK);
  textConfig = textConfigFromColor("menu3", olc::DARK_GREY, TextAlignment::RIGHT);
  menu       = std::make_unique<UiMenu>(config, bgConfig, textConfig);
  menuVertical->addMenu(std::move(menu));

  m_uiMenu->addMenu(std::move(menuVertical));

  bgConfig = bgConfigFromColor(olc::BLACK);
  menu     = std::make_unique<UiMenu>(config, bgConfig);
  m_uiMenu->addMenu(std::move(menu));

  bgConfig   = bgConfigFromColor(olc::DARK_PURPLE);
  textConfig = textConfigFromColor("hihi", olc::DARK_RED, TextAlignment::RIGHT);
  menu       = std::make_unique<UiMenu>(config, bgConfig, textConfig);
  m_uiMenu->addMenu(std::move(menu));
}

bool LoginScreenUiHandler::processUserInput(UserInputData &inputData)
{
  const auto res = m_uiMenu->processUserInput(inputData);
  if (res)
  {
    return res;
  }

  const auto out = m_menu->processUserInput(inputData.controls, inputData.actions);
  return out.relevant;
}

void LoginScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
  m_uiMenu->render(engine.getRenderer());
}

void LoginScreenUiHandler::updateUi() {}

} // namespace pge
