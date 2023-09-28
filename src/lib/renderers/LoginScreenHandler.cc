
#include "LoginScreenHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginScreenHandler::LoginScreenHandler() {}

void LoginScreenHandler::loadResources(int width,
                                       int height,
                                       sprites::TexturePack & /*texturesLoader*/)
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

void LoginScreenHandler::render(SpriteRenderer &engine,
                                const RenderState & /*state*/,
                                const RenderingPass pass) const
{
  switch (pass)
  {
    case RenderingPass::UI:
      m_menu->render(engine.getRenderer());
      break;
    default:
      break;
  }
}

auto LoginScreenHandler::processUserInput(const controls::State &c,
                                          std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void LoginScreenHandler::updateUi() {}

} // namespace pge