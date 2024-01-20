
#include "Game.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "NetworkMessageQueue.hh"
#include "NetworkSystem.hh"

#include "GameScreenInputHandler.hh"
#include "GameScreenRenderer.hh"
#include "GameScreenUiHandler.hh"
#include "LoginScreenRenderer.hh"
#include "LoginScreenUiHandler.hh"
#include "MapScreenRenderer.hh"
#include "MapScreenUiHandler.hh"
#include "OutpostScreenRenderer.hh"
#include "OutpostScreenUiHandler.hh"

#include "DockMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "SlotMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace pge {

Game::Game()
  : utils::CoreObject("game")
  , m_messageModule(this)
{
  setService("game");
  initialize();
}

Game::~Game()
{
  m_messageQueue.reset();
  m_networkContext->stop();
}

auto Game::getScreen() const noexcept -> Screen
{
  return m_state.screen;
}

namespace {
bool shouldUpdateCoordinatorAfterScreenTransition(const Screen &current, const Screen &next)
{
  return Screen::OUTPOST == current && Screen::GAME == next;
}
} // namespace

void Game::setScreen(const Screen &screen)
{
  if (shouldUpdateCoordinatorAfterScreenTransition(m_state.screen, screen))
  {
    info("Resetting game data");
    m_dataSource.initialize(*m_coordinator);
  }

  const auto ui = m_uiHandlers.find(screen);
  if (ui != m_uiHandlers.cend())
  {
    ui->second->reset();
  }

  m_state.screen = screen;
}

void Game::generateRenderers(int width, int height, Renderer &engine)
{
  auto login = std::make_unique<LoginScreenRenderer>();
  login->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);

  auto outpost = std::make_unique<OutpostScreenRenderer>();
  outpost->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::OUTPOST] = std::move(outpost);

  auto game = std::make_unique<GameScreenRenderer>(m_views);
  game->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::GAME] = std::move(game);

  auto map = std::make_unique<MapScreenRenderer>();
  map->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::MAP] = std::move(map);
}

void Game::generateInputHandlers()
{
  m_inputHandlers[Screen::GAME] = std::make_unique<GameScreenInputHandler>(m_views);
}

void Game::generateUiHandlers(int width, int height)
{
  auto login = std::make_unique<LoginScreenUiHandler>(m_views);
  login->initializeMenus(width, height);
  m_uiHandlers[Screen::LOGIN] = std::move(login);

  auto outpost = std::make_unique<OutpostScreenUiHandler>(m_views);
  outpost->initializeMenus(width, height);
  m_uiHandlers[Screen::OUTPOST] = std::move(outpost);

  auto game = std::make_unique<GameScreenUiHandler>(m_views);
  game->initializeMenus(width, height);
  m_uiHandlers[Screen::GAME] = std::move(game);

  auto map = std::make_unique<MapScreenUiHandler>(m_views);
  map->initializeMenus(width, height);
  m_uiHandlers[Screen::MAP] = std::move(map);

  for (const auto &[_, handler] : m_uiHandlers)
  {
    handler->connectToMessageQueue(*m_messageQueue);
  }
}

void Game::processUserInput(const controls::State &controls, CoordinateFrame &frame)
{
  const auto inputHandler = m_inputHandlers.find(getScreen());
  if (inputHandler != m_inputHandlers.end())
  {
    inputHandler->second->processUserInput(controls, frame);
  }

  UserInputData uid{.controls = controls};
  bool userInputRelevant{false};
  const auto uiHandler = m_uiHandlers.find(getScreen());
  if (uiHandler != m_uiHandlers.end())
  {
    userInputRelevant = uiHandler->second->processUserInput(uid);

    for (const auto &action : uid.actions)
    {
      action(*this);
    }
  }

  if (controls.released(controls::mouse::LEFT) && inputHandler != m_inputHandlers.end()
      && !userInputRelevant)
  {
    Vec2f it;
    const auto tp = frame.pixelsToTilesAndIntra(Vec2f(controls.mPosX, controls.mPosY), &it);
    inputHandler->second->performAction(tp.x + it.x, tp.y + it.y, controls);
  }

  if (controls.released(controls::keys::M))
  {
    std::optional<Screen> nextScreen{};
    switch (getScreen())
    {
      case Screen::GAME:
        nextScreen = {Screen::MAP};
        break;
      case Screen::MAP:
        nextScreen = {Screen::GAME};
        break;
      default:
        break;
    }
    if (nextScreen)
    {
      setScreen(*nextScreen);
    }
  }
}

void Game::render(const RenderState &state, const RenderingPass pass) const
{
  const auto itRenderer = m_renderers.find(getScreen());
  const auto itUi       = m_uiHandlers.find(getScreen());

  switch (pass)
  {
    case RenderingPass::SPRITES:
      break;
    case RenderingPass::DECAL:
      if (itRenderer != m_renderers.end())
      {
        itRenderer->second->render(state.renderer, state, pass);
      }
      break;
    case RenderingPass::UI:
      if (itUi != m_uiHandlers.end())
      {
        itUi->second->render(state.renderer);
      }
      break;
    case RenderingPass::DEBUG:
      if (itRenderer != m_renderers.end())
      {
        itRenderer->second->render(state.renderer, state, pass);
      }
      break;
    default:
      warn("Unknown rendering pass " + str(pass));
      break;
  }
}

void Game::terminate() noexcept
{
  info("Game has been terminated");
  m_state.terminated = true;
}

bool Game::terminated() const noexcept
{
  return m_state.terminated;
}

bool Game::step(float elapsedSeconds)
{
  m_coordinator->update(elapsedSeconds);
  m_messageQueue->processMessages();

  const auto it = m_uiHandlers.find(m_state.screen);
  if (it != m_uiHandlers.end())
  {
    it->second->updateUi();
  }

  return true;
}

void Game::login(const bsgo::Uuid &playerDbId)
{
  m_dataSource.setPlayerDbId(playerDbId);
  resetViewsAndUi();
  setScreen(Screen::OUTPOST);
}

void Game::activeShipChanged()
{
  resetViewsAndUi();
}

void Game::activeSystemChanged()
{
  m_views.shipView->clearJumpSystem();
  resetViewsAndUi();
}

void Game::initialize()
{
  const auto repositories = m_dataSource.repositories();

  auto networkSystem = std::make_unique<bsgo::NetworkSystem>(repositories);
  m_networkSystem    = networkSystem.get();
  auto localQueue    = std::make_unique<bsgo::MessageQueue>();
  auto connection    = std::make_unique<bsgo::ClientConnection>(*m_networkContext);
  m_messageQueue     = std::make_unique<bsgo::NetworkMessageQueue>(std::move(localQueue),
                                                               std::move(connection));

  m_coordinator = std::make_shared<bsgo::Coordinator>(std::move(networkSystem),
                                                      m_messageQueue.get());
  m_services    = bsgo::createServices(repositories, m_coordinator);

  initializeViews();
  initializeMessageSystem();

  m_networkContext->start();
}

void Game::initializeViews()
{
  const auto repositories = m_dataSource.repositories();

  m_views.shipView     = std::make_shared<bsgo::ShipView>(m_coordinator,
                                                      repositories,
                                                      m_messageQueue.get());
  m_views.systemView   = std::make_shared<bsgo::SystemView>(m_coordinator,
                                                          repositories,
                                                          m_messageQueue.get());
  m_views.playerView   = std::make_shared<bsgo::PlayerView>(m_coordinator,
                                                          repositories,
                                                          m_messageQueue.get());
  m_views.shopView     = std::make_shared<bsgo::ShopView>(m_coordinator,
                                                      repositories,
                                                      m_messageQueue.get());
  m_views.serverView   = std::make_shared<bsgo::ServerView>(m_coordinator,
                                                          repositories,
                                                          m_messageQueue.get());
  m_views.resourceView = std::make_shared<bsgo::ResourceView>(m_coordinator,
                                                              repositories,
                                                              m_messageQueue.get());
}

void Game::initializeMessageSystem()
{
  auto jump = std::make_unique<bsgo::JumpMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(jump.get());
  m_messageConsumers.emplace_back(std::move(jump));

  auto dock = std::make_unique<bsgo::DockMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(dock.get());
  m_messageConsumers.emplace_back(std::move(dock));

  auto slot = std::make_unique<bsgo::SlotMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(slot.get());
  m_messageConsumers.emplace_back(std::move(slot));

  auto velocity = std::make_unique<bsgo::VelocityMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(velocity.get());
  m_messageConsumers.emplace_back(std::move(velocity));

  auto hangar = std::make_unique<bsgo::HangarMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(hangar.get());
  m_messageConsumers.emplace_back(std::move(hangar));

  auto login = std::make_unique<bsgo::LoginMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(login.get());
  m_messageConsumers.emplace_back(std::move(login));

  auto target = std::make_unique<bsgo::TargetMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(target.get());
  m_messageConsumers.emplace_back(std::move(target));

  auto purchase = std::make_unique<bsgo::PurchaseMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(purchase.get());
  m_messageConsumers.emplace_back(std::move(purchase));

  auto equip = std::make_unique<bsgo::EquipMessageConsumer>(m_services, m_messageQueue.get());
  m_messageQueue->addListener(equip.get());
  m_messageConsumers.emplace_back(std::move(equip));

  m_messageQueue->addListener(&m_messageModule);
}

void Game::resetViewsAndUi()
{
  m_dataSource.initialize(*m_coordinator);

  const auto maybePlayerDbId = m_dataSource.playerDbId();
  if (!maybePlayerDbId)
  {
    error("Failed to reset views and UI", "Expected to have a player defined");
  }

  m_networkSystem->setPlayerDbId(*maybePlayerDbId);

  const auto playerShipDbId     = m_dataSource.playerShipDbId();
  const auto playerShipEntityId = m_dataSource.playerShipEntityId();

  m_views.playerView->setPlayerDbId(*maybePlayerDbId);
  m_views.playerView->setPlayerShipDbId(playerShipDbId);
  m_views.shipView->setPlayerShipDbId(playerShipDbId);
  m_views.shipView->setPlayerShipEntityId(playerShipEntityId);
  m_views.shopView->setPlayerDbId(*maybePlayerDbId);
  m_views.serverView->setPlayerDbId(*maybePlayerDbId);

  m_messageModule.setPlayerShipDbId(playerShipDbId);

  for (const auto &[_, handler] : m_uiHandlers)
  {
    handler->reset();
  }
}

} // namespace pge
