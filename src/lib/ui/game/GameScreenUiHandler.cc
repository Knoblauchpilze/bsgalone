
#include "GameScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

#include "ColorUtils.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
  , m_shipView(views.shipView)
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
  m_menus.resize(MenuItem::COUNT);

  generateShipMenus(width, height);
  generateTargetMenus(width, height);
  generateOutpostMenus(width, height);

  m_weaponsUi->initializeMenus(width, height);
  m_abilitiesUi->initializeMenus(width, height);
  m_gameOverUi->initializeMenus(width, height);
}

bool GameScreenUiHandler::processUserInput(UserInputData &inputData)
{
  auto out = m_weaponsUi->processUserInput(inputData);
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
    for (const auto &menu : m_menus)
    {
      if (menu->processUserInput(inputData))
      {
        return true;
      }
    }
  }

  return out;
}

void GameScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }

  m_weaponsUi->render(engine);
  m_abilitiesUi->render(engine);
  m_gameOverUi->render(engine);
}

void GameScreenUiHandler::updateUi()
{
  updateShipUi();
  updateTargetUi();
  updateOutpostUi();

  m_weaponsUi->updateUi();
  m_abilitiesUi->updateUi();
  m_gameOverUi->updateUi();
}

void GameScreenUiHandler::reset()
{
  m_weaponsUi->reset();
  m_abilitiesUi->reset();
  m_gameOverUi->reset();
}

void GameScreenUiHandler::generateShipMenus(int /*width*/, int /*height*/)
{
  const olc::vi2d SHIP_UI_PIXEL_POS{5, 5};
  const olc::vi2d SHIP_UI_PIXEL_DIMENSION{200, 15};
  constexpr auto REASONABLE_PIXEL_GAP = 15;

  MenuConfig config{.pos           = SHIP_UI_PIXEL_POS,
                    .dims          = SHIP_UI_PIXEL_DIMENSION,
                    .highlightable = false};

  auto bg       = bgConfigFromColor(olc::BLANK);
  auto text     = textConfigFromColor("N/A", olc::WHITE);
  m_menus[NAME] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg              = bgConfigFromColor(olc::VERY_DARK_RED);
  text            = textConfigFromColor("Health: N/A", olc::WHITE);
  m_menus[HEALTH] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg             = bgConfigFromColor(olc::DARK_CYAN);
  text           = textConfigFromColor("Power: N/A", olc::WHITE);
  m_menus[POWER] = std::make_unique<UiTextMenu>(config, bg, text);
}

void GameScreenUiHandler::generateTargetMenus(int width, int /*height*/)
{
  const olc::vi2d TARGET_UI_PIXEL_POS{width / 2, 5};
  const olc::vi2d TARGET_UI_PIXEL_DIMENSION{200, 15};
  constexpr auto REASONABLE_PIXEL_GAP = 15;

  MenuConfig config{.pos           = TARGET_UI_PIXEL_POS,
                    .dims          = TARGET_UI_PIXEL_DIMENSION,
                    .highlightable = false};

  auto bg              = bgConfigFromColor(olc::BLANK);
  auto text            = textConfigFromColor("N/A", olc::WHITE);
  m_menus[TARGET_NAME] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg                     = bgConfigFromColor(olc::VERY_DARK_RED);
  text                   = textConfigFromColor("Health: N/A", olc::WHITE);
  m_menus[TARGET_HEALTH] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg                    = bgConfigFromColor(olc::DARK_CYAN);
  text                  = textConfigFromColor("Power: N/A", olc::WHITE);
  m_menus[TARGET_POWER] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg                       = bgConfigFromColor(olc::BLANK);
  text                     = textConfigFromColor("N/A m", olc::WHITE);
  m_menus[TARGET_DISTANCE] = std::make_unique<UiTextMenu>(config, bg, text);
}

void GameScreenUiHandler::generateOutpostMenus(int width, int /*height*/)
{
  const olc::vi2d OUTPOST_UI_PIXEL_POS{width / 2, 70};
  const olc::vi2d OUTPOST_UI_PIXEL_DIMENSION{100, 25};

  MenuConfig config{.pos               = OUTPOST_UI_PIXEL_POS,
                    .dims              = OUTPOST_UI_PIXEL_DIMENSION,
                    .gameClickCallback = [this](Game &g) {
                      if (m_shipView->isReady())
                      {
                        m_shipView->dockPlayerShip();
                        g.setScreen(Screen::OUTPOST);
                      }
                    }};

  auto bg       = bgConfigFromColor(olc::DARK_GREY);
  auto text     = textConfigFromColor("Dock", olc::WHITE);
  m_menus[DOCK] = std::make_unique<UiTextMenu>(config, bg, text);
}

void GameScreenUiHandler::updateShipUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto ship = m_shipView->getPlayerShip();

  auto text = m_shipView->getPlayerShipName();
  m_menus[NAME]->setText(text);

  text = "Health: ";
  text += floatToStr(std::floor(ship.healthComp().value()), 0);
  text += "/";
  text += floatToStr(ship.healthComp().max(), 0);
  m_menus[HEALTH]->setText(text);

  text = "Health: ";
  text += floatToStr(std::floor(ship.healthComp().value()), 0);
  text += "/";
  text += floatToStr(ship.healthComp().max(), 0);
  m_menus[HEALTH]->setText(text);

  text = "Power: ";
  text += floatToStr(std::floor(ship.powerComp().value()), 0);
  text += "/";
  text += floatToStr(std::floor(ship.powerComp().max()), 0);
  m_menus[POWER]->setText(text);
}

void GameScreenUiHandler::updateTargetUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto target = m_shipView->getPlayerTarget();
  m_menus[TARGET_NAME]->setVisible(target.has_value());
  m_menus[TARGET_HEALTH]->setVisible(target.has_value());
  m_menus[TARGET_POWER]->setVisible(target.has_value());
  m_menus[TARGET_DISTANCE]->setVisible(target.has_value());

  if (!target)
  {
    return;
  }

  std::string text;

  text = m_shipView->getPlayerTargetName().value();
  m_menus[TARGET_NAME]->setText(text);

  text = "Health: ";
  if (!target->exists<bsgo::HealthComponent>())
  {
    text += "N/A";
  }
  else
  {
    text += floatToStr(std::floor(target->healthComp().value()), 0);
    text += "/";
    text += floatToStr(std::floor(target->healthComp().max()), 0);
  }
  m_menus[TARGET_HEALTH]->setText(text);

  text = "Power: ";
  if (!target->exists<bsgo::PowerComponent>())
  {
    text += "N/A";
  }
  else
  {
    text += floatToStr(std::floor(target->powerComp().value()), 0);
    text += "/";
    text += floatToStr(std::floor(target->powerComp().max()), 0);
  }
  m_menus[TARGET_POWER]->setText(text);

  const auto d = m_shipView->distanceToTarget();
  text         = floatToStr(d, 1) + "m";
  m_menus[TARGET_DISTANCE]->setText(text);
}

void GameScreenUiHandler::updateOutpostUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

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

  m_menus[DOCK]->setVisible(dockButtonVisible);
}

} // namespace pge
