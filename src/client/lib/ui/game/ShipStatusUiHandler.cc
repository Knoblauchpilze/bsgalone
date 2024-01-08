
#include "ShipStatusUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

ShipStatusUiHandler::ShipStatusUiHandler(const bsgo::Views &views)
  : IUiHandler("ship_status")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void ShipStatusUiHandler::initializeMenus(const int width, const int height)
{
  initializeThreatPanel(width, height);
  initializeJumpPanel(width, height);
}

bool ShipStatusUiHandler::processUserInput(UserInputData &inputData)
{
  return m_jumpPanel->processUserInput(inputData);
}

void ShipStatusUiHandler::render(Renderer &engine) const
{
  m_threatLabel->render(engine);
  m_jumpPanel->render(engine);
}

void ShipStatusUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  updateThreatPanel();
  updateJumpPanel();
}

namespace {
const Vec2i THREAT_UI_PIXEL_DIMENSION{100, 20};
const Vec2i JUMP_UI_PIXEL_DIMENSION{100, 100};
constexpr auto REASONABLE_GAP_PIXELS = 15;
} // namespace

void ShipStatusUiHandler::initializeThreatPanel(const int width, const int height)
{
  const Vec2i threatPixelPos{(width - THREAT_UI_PIXEL_DIMENSION.x) / 2,
                             height - JUMP_UI_PIXEL_DIMENSION.y - REASONABLE_GAP_PIXELS
                               - THREAT_UI_PIXEL_DIMENSION.y};

  const MenuConfig config{.pos           = threatPixelPos,
                          .dims          = THREAT_UI_PIXEL_DIMENSION,
                          .highlightable = false};

  const auto bg   = bgConfigFromColor(colors::BLANK);
  const auto text = textConfigFromColor("Threat", colors::RED);

  auto label = std::make_unique<UiTextMenu>(config, bg, text);
  BlinkingMenuConfig blinkConfig{};
  blinkConfig.applyToBackground = false;
  m_threatLabel                 = std::make_unique<UiBlinkingMenu>(blinkConfig, std::move(label));
}

void ShipStatusUiHandler::initializeJumpPanel(const int width, const int height)
{
  const Vec2i jumpUiPixelPos{(width - JUMP_UI_PIXEL_DIMENSION.x) / 2,
                             height - JUMP_UI_PIXEL_DIMENSION.y - REASONABLE_GAP_PIXELS};

  const MenuConfig config{.pos           = jumpUiPixelPos,
                          .dims          = JUMP_UI_PIXEL_DIMENSION,
                          .highlightable = false};

  auto bg     = bgConfigFromColor(semiOpaque(colors::DARK_RED));
  m_jumpPanel = std::make_unique<UiMenu>(config, bg);

  bg        = bgConfigFromColor(colors::BLANK);
  auto text = textConfigFromColor("FTL Jump", colors::WHITE);
  auto menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = textConfigFromColor("Destination:", colors::WHITE);
  menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text              = textConfigFromColor("N/A", colors::WHITE);
  menu              = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpDestination = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = textConfigFromColor("Remaining:", colors::WHITE);
  menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text       = textConfigFromColor("N/A", colors::WHITE);
  menu       = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpTime = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());
}

void ShipStatusUiHandler::updateThreatPanel()
{
  m_threatLabel->setVisible(m_shipView->isInThreat());
  m_threatLabel->update();
}

void ShipStatusUiHandler::updateJumpPanel()
{
  const auto jumping = m_shipView->isJumping();
  m_jumpPanel->setVisible(jumping);

  if (!jumping)
  {
    return;
  }

  const auto data = m_shipView->getJumpData();
  m_jumpDestination->setText(data.systemName);
  m_jumpTime->setText(utils::durationToPrettyString(data.jumpTime));
}

} // namespace pge
