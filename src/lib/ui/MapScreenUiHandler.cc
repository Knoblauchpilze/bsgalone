
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"

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
  for (const auto &menu : m_buttons)
  {
    if (menu->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void MapScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_buttons)
  {
    menu->render(engine.getRenderer());
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
  auto quitButton = std::make_unique<UiTextMenu>(config, bg, text);
  m_buttons.push_back(std::move(quitButton));
}

namespace {
auto systemPosToRatio(const bsgo::ServerView::Bounds &bounds, const Eigen::Vector3f &pos)
  -> Eigen::Vector3f
{
  const auto range  = bounds.max - bounds.min;
  const auto offset = pos - bounds.min;

  Eigen::Vector3f out{};

  /// https://stackoverflow.com/questions/25055571/checking-if-all-entries-in-the-matrix-are-zero-in-eigen-library
  constexpr auto IS_ZERO_DIGIT_PRECISION = 0.001f;
  out(0) = offset.row(0).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(0) / range(0);
  out(1) = offset.row(1).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(1) / range(1);
  out(2) = offset.row(2).isZero(IS_ZERO_DIGIT_PRECISION) ? 0 : offset(2) / range(2);

  return out;
}

auto remapRatioToScale(const Eigen::Vector3f &ratio, const float scale) -> Eigen::Vector3f
{
  const Eigen::Vector3f offset = Eigen::Vector3f::Ones() * 0.5f * (1.0f - 1.0f / scale);

  Eigen::Vector3f out = ratio;
  out.noalias()       = out / scale + offset;
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
  const auto bounds  = m_serverView->getMapBounds();
  const auto systems = m_serverView->getAllSystems();

  constexpr auto SYSTEM_BUTTON_SIZE = 10;
  const olc::vi2d systemButtonDimsPixels{SYSTEM_BUTTON_SIZE, SYSTEM_BUTTON_SIZE};

  constexpr auto SERVER_MAP_TO_PIXEL_MAP_SCALE = 1.5f;
  constexpr auto MAP_OFFSET                    = 10;
  const olc::vi2d mapOffset{MAP_OFFSET, MAP_OFFSET};
  const olc::vi2d mapDims{width - 2 * MAP_OFFSET, height - 2 * MAP_OFFSET};

  for (const auto &system : systems)
  {
    const auto posRatio       = systemPosToRatio(bounds, system.position);
    const auto ratioRemapped  = remapRatioToScale(posRatio, SERVER_MAP_TO_PIXEL_MAP_SCALE);
    const auto posPixels      = posRatioToPixelPos(ratioRemapped, mapOffset, mapDims);
    const olc::vi2d buttonPos = posPixels - systemButtonDimsPixels / 2;

    const MenuConfig config{.pos = buttonPos, .dims = systemButtonDimsPixels};

    const auto bg = bgConfigFromColor(olc::BLACK);
    auto button   = std::make_unique<UiMenu>(config, bg);
    m_buttons.push_back(std::move(button));
  }
}

} // namespace pge
