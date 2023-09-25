
#include "MapRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

MapRenderer::MapRenderer(int width, int height)
{
  create(width, height);
}

void MapRenderer::render(SpriteRenderer &engine, const RenderState & /*state*/) const
{
  m_menu->render(engine.getRenderer());
}

auto MapRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void MapRenderer::create(int width, int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_ORANGE);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to game",
                                     olc::VERY_DARK_ORANGE,
                                     "back_to_game",
                                     true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

} // namespace pge
