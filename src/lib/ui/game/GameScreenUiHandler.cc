
#include "GameScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

#include "ColorUtils.hh"

namespace pge {
namespace {
const olc::vi2d SHIP_UI_PIXEL_POS{5, 30};
const olc::vi2d TARGET_UI_PIXEL_POS{400, 30};
} // namespace

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
  , m_shipView(views.shipView)
  , m_shipUi(std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = SHIP_UI_PIXEL_POS,
                                                              .displayDistance = false,
                                                              .getEntity =
                                                                [](const bsgo::ShipView &shipView) {
                                                                  return shipView.getPlayerShip();
                                                                }},
                                               views))
  , m_targetUi(
      std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = TARGET_UI_PIXEL_POS,
                                                       .displayDistance = true,
                                                       .getEntity =
                                                         [](const bsgo::ShipView &shipView) {
                                                           return shipView.getPlayerTarget();
                                                         }},
                                        views))
  , m_weaponsUi(std::make_unique<WeaponsUiHandler>(views))
  , m_abilitiesUi(std::make_unique<AbilitiesUiHandler>(views))
  , m_gameOverUi(std::make_unique<GameOverUiHandler>(views))
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameScreenUiHandler::initializeMenus(const int width, const int height)
{
  generateOutpostMenus(width, height);
  generateJumpMenus(width, height);

  m_shipUi->initializeMenus(width, height);
  m_targetUi->initializeMenus(width, height);
  m_weaponsUi->initializeMenus(width, height);
  m_abilitiesUi->initializeMenus(width, height);
  m_gameOverUi->initializeMenus(width, height);
}

bool GameScreenUiHandler::processUserInput(UserInputData &inputData)
{
  auto out = m_shipUi->processUserInput(inputData);
  if (!out)
  {
    out = m_targetUi->processUserInput(inputData);
  }
  if (!out)
  {
    m_weaponsUi->processUserInput(inputData);
  }
  if (!out)
  {
    out = m_abilitiesUi->processUserInput(inputData);
  }
  if (!out)
  {
    out = m_gameOverUi->processUserInput(inputData);
  }
  if (!out)
  {
    out = m_dock->processUserInput(inputData);
  }
  if (!out)
  {
    out = m_jumpPanel->processUserInput(inputData);
  }

  return out;
}

void GameScreenUiHandler::render(SpriteRenderer &engine) const
{
  m_dock->render(engine.getRenderer());
  m_jumpPanel->render(engine.getRenderer());

  m_shipUi->render(engine);
  m_targetUi->render(engine);
  m_weaponsUi->render(engine);
  m_abilitiesUi->render(engine);
  m_gameOverUi->render(engine);
}

void GameScreenUiHandler::updateUi()
{
  if (m_shipView->isReady())
  {
    updateOutpostUi();
    updateJumpUi();
  }

  m_shipUi->updateUi();
  m_targetUi->updateUi();
  m_weaponsUi->updateUi();
  m_abilitiesUi->updateUi();
  m_gameOverUi->updateUi();
}

void GameScreenUiHandler::reset()
{
  m_shipUi->reset();
  m_targetUi->reset();
  m_weaponsUi->reset();
  m_abilitiesUi->reset();
  m_gameOverUi->reset();
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
