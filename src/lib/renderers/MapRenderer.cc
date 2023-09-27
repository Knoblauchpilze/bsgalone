
#include "MapRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

MapRenderer::MapRenderer() {}

void MapRenderer::loadResources(int width, int height, sprites::TexturePack & /*texturesLoader*/)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_ORANGE);

  MenuShPtr m = generateScreenOption(dims, "Close", olc::VERY_DARK_ORANGE, "close", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

void MapRenderer::render(SpriteRenderer &engine,
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

auto MapRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void MapRenderer::updateUi() {}

} // namespace pge
