
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

MapScreenUiHandler::MapScreenUiHandler()
  : IUiHandler("map")
{}

void MapScreenUiHandler::initializeMenus(const int width, const int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_ORANGE);

  MenuShPtr m = generateScreenOption(dims, "Close", olc::VERY_DARK_ORANGE, "close", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

auto MapScreenUiHandler::processUserInput(const controls::State &c,
                                          std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void MapScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void MapScreenUiHandler::updateUi() {}

} // namespace pge
