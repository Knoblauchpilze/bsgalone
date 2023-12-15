
#include "ShipStatusUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

ShipStatusUiHandler::ShipStatusUiHandler(const bsgo::Views &views)
  : IUiHandler("ship_status")
  , m_shipView(views.shipView)
  , m_serverView(views.serverView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_serverView)
  {
    throw std::invalid_argument("Expected non null server view");
  }
}

namespace {
const olc::vi2d JUMP_UI_PIXEL_DIMENSION{100, 100};
constexpr auto REASONABLE_GAP_PIXELS = 15;
} // namespace

void ShipStatusUiHandler::initializeMenus(const int width, const int height)
{
  const olc::vi2d JUMP_UI_PIXEL_POS{(width - JUMP_UI_PIXEL_DIMENSION.x) / 2,
                                    height - JUMP_UI_PIXEL_DIMENSION.y - REASONABLE_GAP_PIXELS};

  const MenuConfig config{.pos           = JUMP_UI_PIXEL_POS,
                          .dims          = JUMP_UI_PIXEL_DIMENSION,
                          .highlightable = false};

  auto bg     = bgConfigFromColor(transparent(olc::DARK_RED, alpha::SemiOpaque));
  m_jumpPanel = std::make_unique<UiMenu>(config, bg);

  bg        = bgConfigFromColor(olc::BLANK);
  auto text = textConfigFromColor("FTL Jump", olc::WHITE);
  auto menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = textConfigFromColor("Destination:", olc::WHITE);
  menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text              = textConfigFromColor("N/A", olc::WHITE);
  menu              = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpDestination = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());

  text = textConfigFromColor("Remaining:", olc::WHITE);
  menu = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpPanel->addMenu(std::move(menu));

  text       = textConfigFromColor("N/A", olc::WHITE);
  menu       = std::make_unique<UiTextMenu>(config, bg, text);
  m_jumpTime = menu.get();
  m_jumpPanel->addMenu(std::move(menu));

  m_jumpPanel->addMenu(generateSpacer());
}

bool ShipStatusUiHandler::processUserInput(UserInputData &inputData)
{
  return m_jumpPanel->processUserInput(inputData);
}

void ShipStatusUiHandler::render(SpriteRenderer &engine) const
{
  m_jumpPanel->render(engine.getRenderer());
}

void ShipStatusUiHandler::updateUi()
{
  if (!m_serverView->isReady())
  {
    return;
  }

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
