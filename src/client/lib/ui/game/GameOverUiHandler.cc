
#include "GameOverUiHandler.hh"
#include "EntityRemovedMessage.hh"
#include "IViewListenerProxy.hh"
#include "MessageUtils.hh"

namespace pge {

GameOverUiHandler::GameOverUiHandler(const Views &views)
  : IUiHandler("gameover")
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
                                        sprites::TexturePack & /*texturesLoader*/)
{
  Vec2i dims{200, 70};
  Vec2i pos;
  pos.x = (width - dims.x) / 2;
  pos.y = (height - dims.y) / 2;

  const MenuConfig config{.pos               = pos,
                          .dims              = dims,
                          .visible           = false,
                          .gameClickCallback = [this](Game &g) { g.setScreen(Screen::OUTPOST); }};

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

  m_menu->setVisible(m_shipView->isDead());
}

void GameOverUiHandler::reset()
{
  m_menu->setVisible(false);
}

void GameOverUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shipDbView->addListener(std::move(listener));

  listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shipView->addListener(std::move(listener));
}

} // namespace pge
