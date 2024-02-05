
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
  Vec2i dims{200, 70};
  Vec2i pos;
  pos.x = (width - dims.x) / 2;
  pos.y = (height - dims.y) / 2;

  const MenuConfig config{.pos = pos, .dims = dims, .visible = false, .clickCallback = [this]() {
                            if (m_shipView->isReady())
                            {
                              m_shipView->dockPlayerShip();
                            }
                          }};

  auto bg   = bgConfigFromColor(colors::DARK_GREY);
  auto text = textConfigFromColor("Return to outpost", colors::BLACK);
  m_menu    = std::make_unique<UiTextMenu>(config, bg, text);
}

bool GameOverUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void GameOverUiHandler::render(Renderer &engine) const
{
  m_menu->render(engine);
}

void GameOverUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto ship = m_shipView->getPlayerShip();
  m_menu->setVisible(bsgo::Status::DEAD == ship.statusComp().status());
}

} // namespace pge
