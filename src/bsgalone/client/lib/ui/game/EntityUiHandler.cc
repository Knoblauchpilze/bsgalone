
#include "EntityUiHandler.hh"
#include "IViewListenerProxy.hh"
#include "StringUtils.hh"

namespace bsgalone::client {

EntityUiHandler::EntityUiHandler(const EntityUiConfig &config, const Views &views)
  : AbstractUiHandler("ship")
  , m_config(config)
  , m_shipView(views.shipView)
  , m_shipDbView(views.shipDbView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }

  subscribeToViews();
}

void EntityUiHandler::initializeMenus(const int /*width*/,
                                      const int /*height*/,
                                      pge::sprites::TexturePack & /*texturesLoader*/)
{
  m_menus.resize(MenuItem::COUNT);

  const pge::Vec2i TARGET_UI_PIXEL_DIMENSION{200, 15};
  constexpr auto REASONABLE_PIXEL_GAP = 15;

  ui::MenuConfig config{.pos           = m_config.offset,
                        .dims          = TARGET_UI_PIXEL_DIMENSION,
                        .highlightable = false};

  auto bg       = ui::bgConfigFromColor(pge::colors::BLANK);
  auto text     = ui::textConfigFromColor("N/A", pge::colors::WHITE);
  m_menus[NAME] = std::make_unique<ui::UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg              = ui::bgConfigFromColor(pge::colors::VERY_DARK_RED);
  text            = ui::textConfigFromColor("Health: N/A", pge::colors::WHITE);
  m_menus[HEALTH] = std::make_unique<ui::UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg             = ui::bgConfigFromColor(pge::colors::DARK_CYAN);
  text           = ui::textConfigFromColor("Power: N/A", pge::colors::WHITE);
  m_menus[POWER] = std::make_unique<ui::UiTextMenu>(config, bg, text);

  config.pos.y += REASONABLE_PIXEL_GAP;
  bg                = ui::bgConfigFromColor(pge::colors::BLANK);
  text              = ui::textConfigFromColor("N/A m", pge::colors::WHITE);
  m_menus[DISTANCE] = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_menus[DISTANCE]->setVisible(false);

  config.pos.y += REASONABLE_PIXEL_GAP;
  config.highlightable = true;
  config.dims          = pge::Vec2i{100, 25};
  config.clickCallback = [this]() {
    if (m_shipDbView->isReady())
    {
      m_shipDbView->dockPlayerShip();
    }
  };
  bg            = ui::bgConfigFromColor(pge::colors::DARK_GREY);
  text          = ui::textConfigFromColor("Dock", pge::colors::WHITE);
  m_menus[DOCK] = std::make_unique<ui::UiTextMenu>(config, bg, text);
  m_menus[DOCK]->setVisible(false);
}

bool EntityUiHandler::processUserInput(ui::UserInputData &inputData)
{
  auto out{false};
  for (const auto &menu : m_menus)
  {
    out |= menu->processUserInput(inputData);
  }

  return out;
}

void EntityUiHandler::render(pge::Renderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine);
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

void EntityUiHandler::subscribeToViews()
{
  // There's nothing dynamic in this component so there's no need
  // to register this handler as a listener of the views.
  // In case this changes, take inspiration from the other handlers.
}

void EntityUiHandler::updateNameComponent(const core::Entity &entity)
{
  m_menus[NAME]->setText(m_shipView->getEntityName(entity));
}

void EntityUiHandler::updateHealthComponent(const core::Entity &entity)
{
  if (!entity.exists<core::HealthComponent>())
  {
    m_menus[HEALTH]->setText("Health: N/A");
    return;
  }

  std::string text{"Health: "};
  text += core::floatToStr(std::floor(std::max(entity.healthComp().value(), 0.0f)), 0);
  text += "/";
  text += core::floatToStr(std::floor(entity.healthComp().max()), 0);
  m_menus[HEALTH]->setText(text);
}

void EntityUiHandler::updatePowerComponent(const core::Entity &entity)
{
  if (!entity.exists<core::PowerComponent>())
  {
    m_menus[POWER]->setText("Power: N/A");
    return;
  }

  std::string text{"Power: "};
  text += core::floatToStr(std::floor(entity.powerComp().value()), 0);
  text += "/";
  text += core::floatToStr(std::floor(entity.powerComp().max()), 0);
  m_menus[POWER]->setText(text);
}

void EntityUiHandler::updateDistanceComponent()
{
  const auto d = m_shipView->distanceToTarget();
  m_menus[DISTANCE]->setText(core::floatToStr(d, 1) + "m");
}

void EntityUiHandler::updateDockComponent(const core::Entity &entity)
{
  if (core::EntityKind::OUTPOST != entity.kind->kind())
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

} // namespace bsgalone::client
