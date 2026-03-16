
#include "App.hh"
#include "InputGameMessageAdapter.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc, ServerConfig config)
  : PGEApp(desc)
  , m_config(std::move(config))
{}

bool App::onFrame(const float elapsedSeconds)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return false;
  }

  // Process messages first and then update the game so that
  // the systems have a chance to react to messages before the
  // UI is updated.
  m_networkClient->processEvents();

  if (!m_game->step(elapsedSeconds))
  {
    info("This is game over");
  }

  const auto it = m_uiHandlers.find(m_screen);
  if (it != m_uiHandlers.end())
  {
    it->second->updateUi();
  }

  return m_game->terminated();
}

void App::onInputs(const pge::controls::State &controls, pge::CoordinateFrame &frame)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return;
  }

  m_game->processUserInput(controls, frame);
}

void App::loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  setLayerTint(Layer::DRAW, semiOpaque(pge::colors::WHITE));

  m_networkClient = std::make_shared<GameNetworkClient>();
  if (m_config.autoConnect.has_value())
  {
    m_networkClient->setAutoLogin(*m_config.autoConnect);
  }
  m_game = std::make_shared<Game>(m_networkClient);

  m_game->generateRenderers(screenDims.x, screenDims.y, engine);
  m_game->generateInputHandlers();
  m_game->generateUiHandlers(screenDims.x, screenDims.y, engine, m_networkClient);

  initializeMessageSystem();

  m_networkClient->start(m_config.port);
}

void App::cleanResources()
{
  m_networkClient->stop();
  m_networkClient.reset();

  m_game.reset();
}

void App::drawDecal(const pge::RenderState &state)
{
  m_game->render(state, pge::RenderingPass::DECAL);
}

void App::draw(const pge::RenderState &state)
{
  m_game->render(state, pge::RenderingPass::SPRITES);
}

void App::drawUi(const pge::RenderState &state)
{
  m_game->render(state, pge::RenderingPass::UI);
}

void App::drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos)
{
  m_game->render(state, pge::RenderingPass::DEBUG);

  pge::Vec2f it;
  const auto mtp = state.frame.pixelsToTilesAndIntra(mouseScreenPos, &it);

  constexpr auto DEBUG_PIXELS_Y       = 400;
  constexpr auto REASONABLE_PIXEL_GAP = 15;
  pge::Vec2i pos(0, DEBUG_PIXELS_Y);

  state.renderer.drawDebugString(pos,
                                 "Mouse coords      : " + mouseScreenPos.str(),
                                 pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "World cell coords : " + mtp.str(), pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "Intra cell        : " + it.str(), pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos,
                                 "Screen            : " + str(m_game->getScreen()),
                                 pge::colors::DARK_GREEN);
}

void App::initializeMessageSystem()
{
  auto inputAdapter = std::make_unique<InputGameMessageAdapter>();
}

} // namespace bsgalone::client
