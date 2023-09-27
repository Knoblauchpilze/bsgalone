
#include "GameOverRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameOverRenderer::GameOverRenderer(int width, int height)
{
  create(width, height);
}

void GameOverRenderer::render(SpriteRenderer &engine,
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

auto GameOverRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void GameOverRenderer::updateUi() {}

void GameOverRenderer::create(int width, int height)
{
  const olc::vi2d dims{width, height};
  m_menu = generateDefaultScreen(dims, olc::DARK_MAGENTA);

  MenuShPtr m = generateScreenOption(dims,
                                     "Back to outpost",
                                     olc::VERY_DARK_MAGENTA,
                                     "back_to_op",
                                     true);
  m->setSimpleAction([this](Game &g) { g.setScreen(Screen::OUTPOST); });
  m_menu->addMenu(m);

  m = generateScreenOption(dims, "Logout", olc::VERY_DARK_MAGENTA, "logout", true);
  m->setSimpleAction([this](Game &g) {
    g.setScreen(Screen::EXIT);
    g.terminate();
  });
  m_menu->addMenu(m);
}

} // namespace pge