
#include "OutpostScreenHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostScreenHandler::OutpostScreenHandler() {}

void OutpostScreenHandler::loadResources(int width,
                                         int height,
                                         sprites::TexturePack & /*texturesLoader*/)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_YELLOW);

  MenuShPtr m = generateScreenOption(dims, "Undock", olc::VERY_DARK_YELLOW, "undock", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

void OutpostScreenHandler::render(SpriteRenderer &engine,
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

auto OutpostScreenHandler::processUserInput(const controls::State &c,
                                            std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void OutpostScreenHandler::updateUi() {}

void OutpostScreenHandler::performAction(float /*x*/, float /*y*/, const controls::State & /*state*/)
{}

} // namespace pge
