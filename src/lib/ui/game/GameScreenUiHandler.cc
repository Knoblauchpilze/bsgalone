
#include "GameScreenUiHandler.hh"
#include "AbilitiesUiHandler.hh"
#include "EntityUiHandler.hh"
#include "GameOverUiHandler.hh"
#include "ScreenCommon.hh"
#include "StatusUiHandler.hh"
#include "StringUtils.hh"
#include "WeaponsUiHandler.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  initializeUis(views);
}

void GameScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateOutpostMenus(width, height);
  generateJumpMenus(width, height);

  for (const auto &ui : m_uis)
  {
    ui->initializeMenus(width, height);
  }
}

bool GameScreenUiHandler::processUserInput(UserInputData &inputData)
{
  for (const auto &ui : m_uis)
  {
    if (ui->processUserInput(inputData))
    {
      return true;
    }
  }

  if (m_dock->processUserInput(inputData))
  {
    return true;
  }
  return m_jumpPanel->processUserInput(inputData);
}

void GameScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_dock->render(engine.getRenderer());
  m_jumpPanel->render(engine.getRenderer());

  for (const auto &ui : m_uis)
  {
    ui->render(engine);
  }
}

void GameScreenUiHandler::updateUi()
{
  if (m_shipView->isReady())
  {
    updateOutpostUi();
    updateJumpUi();
  }

  for (const auto &ui : m_uis)
  {
    ui->updateUi();
  }
}

void GameScreenUiHandler::reset()
{
  for (const auto &ui : m_uis)
  {
    ui->reset();
  }
}

namespace {
const olc::vi2d STATUS_UI_PIXEL_POS{5, 5};
const olc::vi2d SHIP_UI_PIXEL_POS{5, 30};
const olc::vi2d TARGET_UI_PIXEL_POS{400, 30};
} // namespace

void GameScreenUiHandler::initializeUis(const bsgo::Views &views)
{
  auto statusUi = std::make_unique<StatusUiHandler>(STATUS_UI_PIXEL_POS, views);
  m_uis.emplace_back(std::move(statusUi));

  auto shipUi
    = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = SHIP_UI_PIXEL_POS,
                                                       .displayDistance = false,
                                                       .getEntity =
                                                         [](const bsgo::ShipView &shipView) {
                                                           return shipView.getPlayerShip();
                                                         }},
                                        views);
  m_uis.emplace_back(std::move(shipUi));

  auto targetUi
    = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = TARGET_UI_PIXEL_POS,
                                                       .displayDistance = true,
                                                       .getEntity =
                                                         [](const bsgo::ShipView &shipView) {
                                                           return shipView.getPlayerTarget();
                                                         }},
                                        views);
  m_uis.emplace_back(std::move(targetUi));

  auto weaponsUi = std::make_unique<WeaponsUiHandler>(views);
  m_uis.emplace_back(std::move(weaponsUi));

  auto abilitiesUi = std::make_unique<AbilitiesUiHandler>(views);
  m_uis.emplace_back(std::move(abilitiesUi));

  auto gameOverUi = std::make_unique<GameOverUiHandler>(views);
  m_uis.emplace_back(std::move(gameOverUi));
}

void GameScreenUiHandler::generateOutpostMenus(int width, int /*height*/)
{
  const olc::vi2d OUTPOST_UI_PIXEL_POS{width / 2, 70};
  const olc::vi2d OUTPOST_UI_PIXEL_DIMENSION{100, 25};

  const MenuConfig config{.pos               = OUTPOST_UI_PIXEL_POS,
                          .dims              = OUTPOST_UI_PIXEL_DIMENSION,
                          .gameClickCallback = [this](Game &g) {
                            if (m_shipView->isReady())
                            {
                              m_shipView->dockPlayerShip();
                              g.setScreen(Screen::OUTPOST);
                            }
                          }};

  auto bg   = bgConfigFromColor(olc::DARK_GREY);
  auto text = textConfigFromColor("Dock", olc::WHITE);
  m_dock    = std::make_unique<UiTextMenu>(config, bg, text);
}

void GameScreenUiHandler::generateJumpMenus(int width, int height)
{
  const olc::vi2d JUMP_UI_PIXEL_DIMENSION{100, 100};
  constexpr auto REASONABLE_GAP_PIXELS = 15;
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

void GameScreenUiHandler::updateOutpostUi()
{
  const auto target                       = m_shipView->getPlayerTarget();
  constexpr auto MAXIMUM_DISTANCE_TO_DOCK = 5.0f;
  auto dockButtonVisible{false};
  if (target && bsgo::EntityKind::OUTPOST == target->kind->kind())
  {
    dockButtonVisible = m_shipView->distanceToTarget() <= MAXIMUM_DISTANCE_TO_DOCK;

    const auto ship              = m_shipView->getPlayerShip();
    const auto factionIsMatching = target->factionComp().faction() == ship.factionComp().faction();

    dockButtonVisible &= factionIsMatching;
  }

  m_dock->setVisible(dockButtonVisible);
}

void GameScreenUiHandler::updateJumpUi()
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
