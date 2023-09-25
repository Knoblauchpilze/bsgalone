
#include "OutpostRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostRenderer::OutpostRenderer(int width, int height)
{
  create(width, height);
}

void OutpostRenderer::render(SpriteRenderer &engine, const RenderState & /*state*/) const
{
  m_menu->render(engine.getRenderer());
}

auto OutpostRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void OutpostRenderer::create(int width, int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_YELLOW);

  MenuShPtr m = generateScreenOption(dims, "Undock", olc::VERY_DARK_YELLOW, "undock", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

} // namespace pge
