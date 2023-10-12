
#include "OutpostScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostScreenUiHandler::OutpostScreenUiHandler()
  : IUiHandler("outpost")
{}

void OutpostScreenUiHandler::initializeMenus(const int width, const int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_YELLOW);

  MenuShPtr m = generateScreenOption(dims, "Undock", olc::VERY_DARK_YELLOW, "undock", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

auto OutpostScreenUiHandler::processUserInput(const controls::State &c,
                                              std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void OutpostScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void OutpostScreenUiHandler::updateUi() {}

} // namespace pge
