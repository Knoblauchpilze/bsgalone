
#include "EntityUiHandler.hh"
#include "StringUtils.hh"

namespace pge {

EntityUiHandler::EntityUiHandler(const EntityUiConfig &config, const bsgo::Views &views)
  : IUiHandler("ship")
  , m_config(config)
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void EntityUiHandler::initializeMenus(const int /*width*/, const int /*height*/)
{
  m_menus.resize(MenuItem::COUNT);

  const olc::vi2d TARGET_UI_PIXEL_DIMENSION{200, 15};
  constexpr auto REASONABLE_PIXEL_GAP = 15;

  MenuConfig config{.pos           = m_config.offset,
                    .dims          = TARGET_UI_PIXEL_DIMENSION,
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

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg                = bgConfigFromColor(olc::BLANK);
  text              = textConfigFromColor("N/A m", olc::WHITE);
  m_menus[DISTANCE] = std::make_unique<UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  config.highlightable     = true;
  config.dims              = olc::vi2d{100, 25};
  config.gameClickCallback = [this](Game &g) {
    if (m_shipView->isReady())
    {
      m_shipView->dockPlayerShip();
      g.setScreen(Screen::OUTPOST);
    }
  };
  bg            = bgConfigFromColor(olc::DARK_GREY);
  text          = textConfigFromColor("Dock", olc::WHITE);
  m_menus[DOCK] = std::make_unique<UiTextMenu>(config, bg, text);
}

bool EntityUiHandler::processUserInput(UserInputData &inputData)
{
  for (const auto &menu : m_menus)
  {
    if (menu->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void EntityUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void EntityUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto maybeShip = m_config.getEntity(*m_shipView);

  m_menus[NAME]->setVisible(maybeShip.has_value());
  m_menus[HEALTH]->setVisible(maybeShip.has_value());
  m_menus[POWER]->setVisible(maybeShip.has_value());
  m_menus[DISTANCE]->setVisible(m_config.displayDistance && maybeShip.has_value());

  if (!maybeShip)
  {
    m_menus[DOCK]->setVisible(false);
    return;
  }

  const auto ship = *maybeShip;
  updateNameComponent(ship);
  updateHealthComponent(ship);
  updatePowerComponent(ship);
  if (m_config.displayDistance)
  {
    updateDistanceComponent();
  }
  updateDockComponent(ship);
}

void EntityUiHandler::updateNameComponent(const bsgo::Entity &entity)
{
  m_menus[NAME]->setText(m_shipView->getEntityName(entity));
}

void EntityUiHandler::updateHealthComponent(const bsgo::Entity &entity)
{
  if (!entity.exists<bsgo::HealthComponent>())
  {
    m_menus[HEALTH]->setText("Health: N/A");
    return;
  }

  std::string text{"Health: "};
  text += floatToStr(std::floor(entity.healthComp().value()), 0);
  text += "/";
  text += floatToStr(std::floor(entity.healthComp().max()), 0);
  m_menus[HEALTH]->setText(text);
}

void EntityUiHandler::updatePowerComponent(const bsgo::Entity &entity)
{
  if (!entity.exists<bsgo::PowerComponent>())
  {
    m_menus[POWER]->setText("Power: N/A");
    return;
  }

  std::string text{"Power: "};
  text += floatToStr(std::floor(entity.powerComp().value()), 0);
  text += "/";
  text += floatToStr(std::floor(entity.powerComp().max()), 0);
  m_menus[POWER]->setText(text);
}

void EntityUiHandler::updateDistanceComponent()
{
  const auto d = m_shipView->distanceToTarget();
  m_menus[DISTANCE]->setText(floatToStr(d, 1) + "m");
}

void EntityUiHandler::updateDockComponent(const bsgo::Entity &entity)
{
  if (bsgo::EntityKind::OUTPOST != entity.kind->kind())
  {
    m_menus[DOCK]->setVisible(false);
    return;
  }

  constexpr auto MAXIMUM_DISTANCE_TO_DOCK = 5.0f;
  auto dockButtonVisible = m_shipView->distanceToTarget() <= MAXIMUM_DISTANCE_TO_DOCK;

  const auto playerShip        = m_shipView->getPlayerShip();
  const auto factionIsMatching = entity.factionComp().faction()
                                 == playerShip.factionComp().faction();

  dockButtonVisible &= factionIsMatching;

  m_menus[DOCK]->setVisible(dockButtonVisible);
}

} // namespace pge
