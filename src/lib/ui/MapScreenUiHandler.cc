
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"

#include "VectorUtils.hh"
#include <iostream>

namespace pge {

MapScreenUiHandler::MapScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("map")
  , m_serverView(views.serverView)
{
  if (nullptr == m_serverView)
  {
    throw std::invalid_argument("Expected non null server view");
  }
}

void MapScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateQuitButton(width, height);
  generateSystemButtons(width, height);
}

bool MapScreenUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_quitButton->processUserInput(inputData))
  {
    return true;
  }

  for (const auto &system : m_systemButtons)
  {
    if (system->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void MapScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_quitButton->render(engine.getRenderer());
  for (const auto &system : m_systemButtons)
  {
    system->render(engine.getRenderer());
  }
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

namespace {
auto systemPosToRatio(const bsgo::ServerView::Bounds &bounds, const Eigen::Vector3f &pos)
  -> Eigen::Vector3f
{
  const auto range  = bounds.max - bounds.min;
  const auto offset = pos - bounds.min;

  constexpr auto IS_ZERO_DIGIT_PRECISION = 0.001f;
  std::cout << "range: " << range << ", offset: " << offset
            << ", check: " << offset.row(0).isZero(IS_ZERO_DIGIT_PRECISION)
            << " because of row(0) = " << offset.row(0) << ", row(1) = " << offset.row(1)
            << std::endl;

  Eigen::Vector3f out{};

  /// https://stackoverflow.com/questions/25055571/checking-if-all-entries-in-the-matrix-are-zero-in-eigen-library
  out(0) = offset.row(0).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(0) / range(0);
  out(1) = offset.row(1).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(1) / range(1);
  out(2) = offset.row(2).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(2) / range(2);

  return out;
}

auto posRatioToPixelPos(const Eigen::Vector3f &posRatio,
                        const olc::vi2d &offset,
                        const olc::vi2d &dims) -> olc::vi2d
{
  olc::vi2d out{};

  out.x = offset.x + static_cast<int>(posRatio(0) * dims.x);
  out.y = offset.y + static_cast<int>(posRatio(1) * dims.y);

  return out;
}
} // namespace

void MapScreenUiHandler::generateSystemButtons(const int width, const int height)
{
  auto bounds = m_serverView->getMapBounds();

  constexpr auto BOUNDS_TO_SCREEN_RATIO = 2.0f;
  bounds.min *= BOUNDS_TO_SCREEN_RATIO;
  bounds.max *= BOUNDS_TO_SCREEN_RATIO;

  log("min: " + bsgo::str(bounds.min) + ", max: " + bsgo::str(bounds.max));

  const auto systems = m_serverView->getAllSystems();

  constexpr auto SYSTEM_BUTTON_SIZE = 10;
  const olc::vi2d systemButtonDimsPixels{SYSTEM_BUTTON_SIZE, SYSTEM_BUTTON_SIZE};

  constexpr auto MAP_OFFSET = 10;
  const olc::vi2d mapOffset{MAP_OFFSET, MAP_OFFSET};
  const olc::vi2d mapDims{width - 2 * MAP_OFFSET, height - 2 * MAP_OFFSET};

  for (const auto &system : systems)
  {
    const auto posRatio       = systemPosToRatio(bounds, system.position);
    const auto posPixels      = posRatioToPixelPos(posRatio, mapOffset, mapDims);
    const olc::vi2d buttonPos = posPixels - systemButtonDimsPixels / 2;

    const MenuConfig config{.pos = buttonPos, .dims = systemButtonDimsPixels};

    log("system " + system.name + " is at " + bsgo::str(system.position) + " with ratio "
        + bsgo::str(posRatio) + " and to " + posPixels.str() + ", dims: " + mapDims.str());

    const auto bg = bgConfigFromColor(olc::VERY_DARK_COBALT_BLUE);
    auto button   = std::make_unique<UiMenu>(config, bg);
    m_systemButtons.push_back(std::move(button));
  }
}

} // namespace pge
