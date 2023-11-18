
#include "GameOverScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

GameOverScreenUiHandler::GameOverScreenUiHandler()
  : IUiHandler("gameover")
{}

void GameOverScreenUiHandler::initializeMenus(const int width, const int height)
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

bool GameOverScreenUiHandler::processUserInput(UserInputData &inputData)
{
  const auto out = m_menu->processUserInput(inputData.controls, inputData.actions);
  return out.relevant;
}

void GameOverScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void GameOverScreenUiHandler::updateUi() {}

} // namespace pge
