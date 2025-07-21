
#include "MapScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "UiTextMenu.hh"

namespace pge {
namespace {
constexpr auto SYSTEM_LABEL_DEFAULT_BG_COLOR  = colors::DARK_GREY;
constexpr auto SYSTEM_LABEL_SELECTED_BG_COLOR = colors::DARK_GREEN;
constexpr auto SYSTEM_LABEL_CURRENT_BG_COLOR  = colors::DARK_CYAN;

constexpr auto MAP_OFFSET = 10;
} // namespace

MapScreenUiHandler::MapScreenUiHandler(const Views &views)
  : IUiHandler("map")
  , m_serverView(views.serverView)
  , m_shipView(views.shipView)
  , m_shipDbView(views.shipDbView)
{
  if (nullptr == m_serverView)
  {
    throw std::invalid_argument("Expected non null server view");
  }
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
}

void MapScreenUiHandler::initializeMenus(const int width,
                                         const int height,
                                         sprites::TexturePack & /*texturesLoader*/)
{
  generateControlButtons(width, height);

  const Vec2i mapOffset{MAP_OFFSET, MAP_OFFSET};
  m_mapDimensions = Vec2i{width - 2 * MAP_OFFSET, height - 2 * MAP_OFFSET};
}

bool MapScreenUiHandler::processUserInput(UserInputData &inputData)
{
  auto out{false};

  for (const auto &menu : m_buttons)
  {
    out |= menu->processUserInput(inputData);
  }

  if (!out && m_selectedSystem && inputData.controls.released(controls::mouse::LEFT))
  {
    debug("reset");
    m_selectedSystem.reset();
  }

  return out;
}

void MapScreenUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_buttons)
  {
    menu->render(engine);
  }
}

void MapScreenUiHandler::updateUi()
{
  if (!m_serverView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    generateMap();
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

void MapScreenUiHandler::reset()
{
  m_selectedSystem.reset();
  m_initialized = false;
}

void MapScreenUiHandler::generateControlButtons(const int width, const int height)
{
  constexpr auto REASONABLE_GAP_SIZE = 20;
  const Vec2i controlButtonDimsPixels{100, 30};
  const Vec2i buttonPos{width - REASONABLE_GAP_SIZE - controlButtonDimsPixels.x,
                        height - REASONABLE_GAP_SIZE - controlButtonDimsPixels.y};

  MenuConfig config{.pos               = buttonPos,
                    .dims              = controlButtonDimsPixels,
                    .gameClickCallback = [this](Game &g) { g.setScreen(Screen::GAME); }};

  auto bg         = bgConfigFromColor(colors::VERY_DARK_COBALT_BLUE);
  auto text       = textConfigFromColor("Close", colors::WHITE);
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

void MapScreenUiHandler::generateMap()
{
  const auto bounds  = m_serverView->getMapBounds();
  const auto systems = m_serverView->getAllSystems();

  const Vec2i mapOffset{MAP_OFFSET, MAP_OFFSET};

  for (const auto &system : systems)
  {
    generateSystemButtons(system, bounds, mapOffset);
  }

  m_initialized = true;
}

namespace {
auto systemPosToRatio(const ServerView::Bounds &bounds, const Eigen::Vector3f &pos)
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

auto posRatioToPixelPos(const Eigen::Vector3f &posRatio, const Vec2i &offset, const Vec2i &dims)
  -> Vec2i
{
  Vec2i out{};
  out.x = offset.x + static_cast<int>(posRatio(0) * dims.x);
  out.y = offset.y + static_cast<int>(posRatio(1) * dims.y);
  return out;
}
} // namespace

void MapScreenUiHandler::generateSystemButtons(const bsgo::SystemData &system,
                                               const ServerView::Bounds &bounds,
                                               const Vec2i &mapOffset)
{
  constexpr auto SERVER_MAP_TO_PIXEL_MAP_SCALE = 1.5f;
  const Vec2i systemButtonDimsPixels{10, 10};

  const auto posRatio      = systemPosToRatio(bounds, system.position);
  const auto ratioRemapped = remapRatioToScale(posRatio, SERVER_MAP_TO_PIXEL_MAP_SCALE);
  const auto posPixels     = posRatioToPixelPos(ratioRemapped, mapOffset, m_mapDimensions);
  const Vec2i buttonPos    = posPixels - systemButtonDimsPixels / 2;

  const auto systemId                 = system.dbId;
  constexpr auto OFFSET_DUE_TO_BUTTON = 1;
  const auto labelId                  = m_buttons.size() + OFFSET_DUE_TO_BUTTON;
  const MenuConfig buttonConfig{.pos           = buttonPos,
                                .dims          = systemButtonDimsPixels,
                                .clickCallback = [this, systemId, labelId]() {
                                  onSystemSelected(systemId, labelId);
                                }};
  const auto buttonBg = bgConfigFromColor(colors::DARK_CYAN);
  auto button         = std::make_unique<UiMenu>(buttonConfig, buttonBg);
  m_buttons.push_back(std::move(button));

  constexpr auto LABEL_BUTTON_WIDTH  = 100;
  constexpr auto LABEL_BUTTON_HEIGHT = 20;
  const Vec2i labelDimsPixels{LABEL_BUTTON_WIDTH, LABEL_BUTTON_HEIGHT};

  constexpr auto GAP_BETWEEN_BUTTON_AND_LABEL_PIXELS = 5;
  const Vec2i labelPos{posPixels.x - LABEL_BUTTON_WIDTH / 2,
                       posPixels.y + systemButtonDimsPixels.y / 2
                         + GAP_BETWEEN_BUTTON_AND_LABEL_PIXELS};

  const MenuConfig labelConfig{.pos = labelPos, .dims = labelDimsPixels, .highlightable = false};
  const auto labelBg         = bgConfigFromColor(SYSTEM_LABEL_DEFAULT_BG_COLOR);
  const auto text            = textConfigFromColor(system.name, colors::WHITE);
  auto label                 = std::make_unique<UiTextMenu>(labelConfig, labelBg, text);
  m_systemMenus[system.dbId] = label.get();
  m_buttons.push_back(std::move(label));
}

void MapScreenUiHandler::onSystemSelected(const bsgo::Uuid systemId, const int labelId)
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
  if (!m_shipDbView->isReady() || !m_shipView->isReady())
  {
    return;
  }

  const auto systemId = m_selectedSystem->systemId;

  m_shipView->setJumpSystem(systemId);
  m_shipDbView->setJumpSystem(systemId);
  m_shipDbView->startJump();
}

} // namespace pge
