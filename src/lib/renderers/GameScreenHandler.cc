
#include "GameScreenHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

GameScreenHandler::GameScreenHandler(const bsgo::Views &views)
  : m_uiHandler(std::make_unique<GameScreenUiHandler>(views))
  , m_systemHandler(std::make_unique<GameScreenSystemHandler>(views))
  , m_shipHandler(std::make_unique<GameScreenShipHandler>(views))
{}

void GameScreenHandler::loadResources(int width, int height, sprites::TexturePack &texturesLoader)
{
  m_uiHandler->loadResources(width, height, texturesLoader);
  m_systemHandler->loadResources(width, height, texturesLoader);
  m_shipHandler->loadResources(width, height, texturesLoader);
}

void GameScreenHandler::render(SpriteRenderer &engine,
                               const RenderState &state,
                               const RenderingPass pass) const
{
  switch (pass)
  {
    case RenderingPass::UI:
      m_uiHandler->render(engine, state, pass);
      break;
    case RenderingPass::DECAL:
      m_systemHandler->render(engine, state, pass);
      m_shipHandler->render(engine, state, pass);
      break;
    case RenderingPass::DEBUG:
      m_shipHandler->render(engine, state, pass);
      break;
    default:
      break;
  }
}

auto GameScreenHandler::processUserInput(const controls::State &c,
                                         std::vector<ActionShPtr> &actions,
                                         CoordinateFrame &frame) -> menu::InputHandle
{
  const auto systemHandlerOutput = m_systemHandler->processUserInput(c, actions, frame);
  const auto shipHandlerOutput   = m_shipHandler->processUserInput(c, actions, frame);
  const auto uiHandlerOutput     = m_uiHandler->processUserInput(c, actions, frame);

  menu::InputHandle out{};
  out.relevant = systemHandlerOutput.relevant || shipHandlerOutput.relevant
                 || uiHandlerOutput.relevant;
  out.selected = systemHandlerOutput.selected || shipHandlerOutput.selected
                 || uiHandlerOutput.selected;
  return out;
}

void GameScreenHandler::updateUi()
{
  m_uiHandler->updateUi();
}

void GameScreenHandler::performAction(float x, float y, const controls::State &state)
{
  m_systemHandler->performAction(x, y, state);
  m_shipHandler->performAction(x, y, state);
}

} // namespace pge
