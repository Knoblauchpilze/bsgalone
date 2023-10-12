
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
}

auto LoginScreenUiHandler::processUserInput(const controls::State &c,
                                            std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void LoginScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void LoginScreenUiHandler::updateUi() {}

} // namespace pge
