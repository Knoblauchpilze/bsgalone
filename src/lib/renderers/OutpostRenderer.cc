
#include "OutpostRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

OutpostRenderer::OutpostRenderer(bsgo::OutpostViewPtr outpostView, int width, int height)
  : m_outpostView(std::move(outpostView))
{
  if (nullptr == m_outpostView)
  {
    throw std::invalid_argument("Expected non null outpost view");
  }
  create(width, height);
}

void OutpostRenderer::render(SpriteRenderer &engine,
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

auto OutpostRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void OutpostRenderer::updateUi() {}

void OutpostRenderer::create(int width, int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_YELLOW);

  MenuShPtr m = generateScreenOption(dims, "Undock", olc::VERY_DARK_YELLOW, "undock", true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::GAME); });
  m_menu->addMenu(m);
}

} // namespace pge
