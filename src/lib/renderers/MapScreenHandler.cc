
#include "MapScreenHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

MapScreenHandler::MapScreenHandler() {}

void MapScreenHandler::loadResources(int width,
                                     int height,
                                     sprites::TexturePack & /*texturesLoader*/)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_ORANGE);

  MenuShPtr m = generateScreenOption(dims, "Close", olc::VERY_DARK_ORANGE, "close", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

void MapScreenHandler::render(SpriteRenderer &engine,
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

auto MapScreenHandler::processUserInput(const controls::State &c,
                                        std::vector<ActionShPtr> &actions,
                                        CoordinateFrame & /*frame*/) -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void MapScreenHandler::updateUi() {}

void MapScreenHandler::performAction(float /*x*/, float /*y*/, const controls::State & /*state*/) {}

} // namespace pge
