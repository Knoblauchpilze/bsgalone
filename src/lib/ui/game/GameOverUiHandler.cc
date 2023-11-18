
#include "GameOverUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameOverUiHandler::GameOverUiHandler(const bsgo::Views &views)
  : IUiHandler("gameover")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameOverUiHandler::initializeMenus(const int width, const int height)
{
  olc::vi2d dims{200, 70};
  olc::vi2d pos;
  pos.x = (width - dims.x) / 2;
  pos.y = (height - dims.y) / 2;

  m_menu = generateMenu(pos,
                        dims,
                        "Return to outpost",
                        "respawn",
                        olc::VERY_DARK_GREY,
                        {olc::WHITE},
                        true);
  m_menu->setSimpleAction([this](Game &g) {
    m_shipView->dockPlayerShip();
    g.setScreen(Screen::OUTPOST);
  });
  m_menu->setVisible(false);
}

bool GameOverUiHandler::processUserInput(UserInputData &inputData)
{
  const auto out = m_menu->processUserInput(inputData.controls, inputData.actions);
  return out.relevant;
}

void GameOverUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void GameOverUiHandler::updateUi()
{
  const auto ship = m_shipView->getPlayerShip();

  m_menu->setVisible(bsgo::Status::DEAD == ship.statusComp().status());
}

} // namespace pge
