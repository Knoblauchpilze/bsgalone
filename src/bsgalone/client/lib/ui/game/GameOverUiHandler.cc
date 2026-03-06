
#include "GameOverUiHandler.hh"
#include "IViewListenerProxy.hh"
#include "MessageUtils.hh"

namespace bsgalone::client {

GameOverUiHandler::GameOverUiHandler(const Views &views)
  : AbstractUiHandler("gameover")
  , m_shipDbView(views.shipDbView)
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }

  subscribeToViews();
}

void GameOverUiHandler::initializeMenus(const int width,
                                        const int height,
                                        pge::sprites::TexturePack & /*texturesLoader*/)
{
  pge::Vec2i dims{200, 70};
  pge::Vec2i pos;
  pos.x = (width - dims.x) / 2;
  pos.y = (height - dims.y) / 2;

  const ui::MenuConfig config{
    .pos     = pos,
    .dims    = dims,
    .visible = false,
    .clickCallback =
      [this]() {
        if (m_shipDbView->isReady())
        {
          m_shipDbView->returnToOutpost();
        }
      },
  };

  auto bg   = ui::bgConfigFromColor(pge::colors::DARK_GREY);
  auto text = ui::textConfigFromColor("Return to outpost", pge::colors::BLACK);
  m_menu    = std::make_unique<ui::UiTextMenu>(config, bg, text);
}

bool GameOverUiHandler::processUserInput(ui::UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void GameOverUiHandler::render(pge::Renderer &engine) const
{
  m_menu->render(engine);
}

void GameOverUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  m_menu->setVisible(m_shipView->isDead());
}

void GameOverUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shipDbView->addListener(std::move(listener));

  listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shipView->addListener(std::move(listener));
}

void GameOverUiHandler::reset()
{
  m_menu->setVisible(false);
}

} // namespace bsgalone::client
