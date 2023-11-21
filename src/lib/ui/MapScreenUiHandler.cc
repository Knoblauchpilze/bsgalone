
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

MapScreenUiHandler::MapScreenUiHandler()
  : IUiHandler("map")
{}

void MapScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateQuitButton(width, height);
}

bool MapScreenUiHandler::processUserInput(UserInputData &inputData)
{
  return m_quitButton->processUserInput(inputData);
}

void MapScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_quitButton->render(engine.getRenderer());
}

void MapScreenUiHandler::updateUi() {}

void MapScreenUiHandler::generateQuitButton(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const olc::vi2d quitButtonDimsPixels{50, 20};
  const olc::vi2d quitButtonPos{width - REASONABLE_GAP_SIZE - quitButtonDimsPixels.x,
                                height - REASONABLE_GAP_SIZE - quitButtonDimsPixels.y};

  const MenuConfig config{.pos               = quitButtonPos,
                          .dims              = quitButtonDimsPixels,
                          .gameClickCallback = [](Game &g) { g.setScreen(Screen::GAME); }};

  const auto bg   = bgConfigFromColor(olc::VERY_DARK_COBALT_BLUE);
  const auto text = textConfigFromColor("Close", olc::WHITE);
  m_quitButton    = std::make_unique<UiTextMenu>(config, bg, text);
}

} // namespace pge
