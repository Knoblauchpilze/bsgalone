
#include "GameOverUiHandler.hh"
#include "EntityDiedMessage.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"

namespace pge {

GameOverUiHandler::GameOverUiHandler(const bsgo::Views &views)
  : IUiHandler("gameover")
  , AbstractMessageListener({bsgo::MessageType::ENTITY_DIED})
  , m_shipDbView(views.shipDbView)
{
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
}

void GameOverUiHandler::initializeMenus(const int width, const int height)
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
  // Intentionally empty.
}

void GameOverUiHandler::reset()
{
  m_menu->setVisible(false);
}

void GameOverUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void GameOverUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (!m_shipDbView->isReady())
  {
    return;
  }

  m_menu->setVisible(didPlayerShipDied(message.as<bsgo::EntityDiedMessage>(), *m_shipDbView));
}

} // namespace pge
