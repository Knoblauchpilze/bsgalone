
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace pge {
namespace {
const auto SYSTEM_LABEL_DEFAULT_BG_COLOR  = olc::DARK_GREY;
const auto SYSTEM_LABEL_SELECTED_BG_COLOR = olc::DARK_GREEN;
const auto SYSTEM_LABEL_CURRENT_BG_COLOR  = olc::DARK_CYAN;
} // namespace

MapScreenUiHandler::MapScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("map")
  , m_serverView(views.serverView)
  , m_shipView(views.shipView)
{
  if (nullptr == m_serverView)
  {
    throw std::invalid_argument("Expected non null server view");
  }
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void MapScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateControlButtons(width, height);
  generateMap(width, height);
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

  if (m_selectedSystem && inputData.controls.released(controls::mouse::LEFT))
  {
    m_selectedSystem.reset();
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

void MapScreenUiHandler::updateUi()
{
  if (!m_serverView->isReady())
  {
    return;
  }

  m_jumpButton->setVisible(m_selectedSystem.has_value());

  const auto currentSystem = m_serverView->getPlayerSystem();
  for (const auto &[systemId, menu] : m_systemMenus)
  {
    const auto isSelected = m_selectedSystem && (systemId == m_selectedSystem->systemId);
    if (isSelected)
    {
      menu->updateBgColor(SYSTEM_LABEL_SELECTED_BG_COLOR);
    }
    else if (currentSystem == systemId)
    {
      menu->updateBgColor(SYSTEM_LABEL_CURRENT_BG_COLOR);
    }
    else
    {
      menu->updateBgColor(SYSTEM_LABEL_DEFAULT_BG_COLOR);
    }
  }
}

void MapScreenUiHandler::generateControlButtons(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const olc::vi2d controlButtonDimsPixels{100, 30};
  const olc::vi2d buttonPos{width - REASONABLE_GAP_SIZE - controlButtonDimsPixels.x,
                            height - REASONABLE_GAP_SIZE - controlButtonDimsPixels.y};

  MenuConfig config{.pos               = buttonPos,
                    .dims              = controlButtonDimsPixels,
                    .gameClickCallback = [this](Game &g) { g.setScreen(Screen::GAME); }};

  auto bg         = bgConfigFromColor(toOlcPixel(Color::VERY_DARK_COBALT_BLUE));
  auto text       = textConfigFromColor("Close", olc::WHITE);
  auto quitButton = std::make_unique<UiTextMenu>(config, bg, text);
  m_buttons.push_back(std::move(quitButton));

  config.visible = false;
  config.pos.y -= (REASONABLE_GAP_SIZE + controlButtonDimsPixels.y);
  config.clickCallback = [this]() { onJumpRequested(); };

  text.text       = "Jump";
  auto jumpButton = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpButton    = jumpButton.get();
  m_buttons.push_back(std::move(jumpButton));
}

void MapScreenUiHandler::generateMap(const int width, const int height)
{
  const auto bounds  = m_serverView->getMapBounds();
  const auto systems = m_serverView->getAllSystems();

  constexpr auto MAP_OFFSET = 10;
  const olc::vi2d mapOffset{MAP_OFFSET, MAP_OFFSET};
  const olc::vi2d mapDims{width - 2 * MAP_OFFSET, height - 2 * MAP_OFFSET};

  for (const auto &system : systems)
  {
    generateSystemButtons(system, bounds, mapOffset, mapDims);
  }
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

void MapScreenUiHandler::generateSystemButtons(const bsgo::System &system,
                                               const bsgo::ServerView::Bounds &bounds,
                                               const olc::vi2d &mapOffset,
                                               const olc::vi2d &mapDims)
{
  constexpr auto SERVER_MAP_TO_PIXEL_MAP_SCALE = 1.5f;
  constexpr auto SYSTEM_BUTTON_SIZE            = 10;
  const olc::vi2d systemButtonDimsPixels{SYSTEM_BUTTON_SIZE, SYSTEM_BUTTON_SIZE};

  const auto posRatio       = systemPosToRatio(bounds, system.position);
  const auto ratioRemapped  = remapRatioToScale(posRatio, SERVER_MAP_TO_PIXEL_MAP_SCALE);
  const auto posPixels      = posRatioToPixelPos(ratioRemapped, mapOffset, mapDims);
  const olc::vi2d buttonPos = posPixels - systemButtonDimsPixels / 2;

  const auto systemId                 = system.id;
  constexpr auto OFFSET_DUE_TO_BUTTON = 1;
  const auto labelId                  = m_buttons.size() + OFFSET_DUE_TO_BUTTON;
  const MenuConfig buttonConfig{.pos           = buttonPos,
                                .dims          = systemButtonDimsPixels,
                                .clickCallback = [this, systemId, labelId]() {
                                  onSystemSelected(systemId, labelId);
                                }};
  const auto buttonBg = bgConfigFromColor(olc::DARK_CYAN);
  auto button         = std::make_unique<UiMenu>(buttonConfig, buttonBg);
  m_buttons.push_back(std::move(button));

  constexpr auto LABEL_BUTTON_WIDTH  = 100;
  constexpr auto LABEL_BUTTON_HEIGHT = 20;
  const olc::vi2d labelDimsPixels{LABEL_BUTTON_WIDTH, LABEL_BUTTON_HEIGHT};

  constexpr auto GAP_BETWEEN_BUTTON_AND_LABEL_PIXELS = 5;
  const olc::vi2d labelPos{posPixels.x - LABEL_BUTTON_WIDTH / 2,
                           posPixels.y + systemButtonDimsPixels.y / 2
                             + GAP_BETWEEN_BUTTON_AND_LABEL_PIXELS};

  const MenuConfig labelConfig{.pos = labelPos, .dims = labelDimsPixels, .highlightable = false};
  const auto labelBg       = bgConfigFromColor(SYSTEM_LABEL_DEFAULT_BG_COLOR);
  const auto text          = textConfigFromColor(system.name, olc::WHITE);
  auto label               = std::make_unique<UiTextMenu>(labelConfig, labelBg, text);
  m_systemMenus[system.id] = label.get();
  m_buttons.push_back(std::move(label));
}

void MapScreenUiHandler::onSystemSelected(const bsgo::Uuid &systemId, const int labelId)
{
  if (!m_serverView->isReady())
  {
    return;
  }

  if (systemId == m_serverView->getPlayerSystem())
  {
    return;
  }
  m_selectedSystem = SelectedSystem{.systemId = systemId, .labelId = labelId};
}

void MapScreenUiHandler::onJumpRequested()
{
  if (!m_selectedSystem)
  {
    error("Failed to start jump", "No selected system");
  }
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto systemId = m_selectedSystem->systemId;
  m_shipView->setJumpSystem(systemId);
  m_shipView->startJump();
}

} // namespace pge
