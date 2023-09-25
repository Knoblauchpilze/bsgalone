
#include "LoginRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

LoginRenderer::LoginRenderer(int width, int height)
{
  create(width, height);
}

void LoginRenderer::render(SpriteRenderer &engine, const RenderState & /*state*/) const
{
  m_menu->render(engine.getRenderer());
}

auto LoginRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void LoginRenderer::create(int width, int height)
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

} // namespace pge