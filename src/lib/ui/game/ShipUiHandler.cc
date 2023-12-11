
#include "ShipUiHandler.hh"
#include "StringUtils.hh"

namespace pge {

ShipUiHandler::ShipUiHandler(const ShipUiConfig &config, const bsgo::Views &views)
  : IUiHandler("ship")
  , m_config(config)
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void ShipUiHandler::initializeMenus(const int /*width*/, const int /*height*/)
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
}

bool ShipUiHandler::processUserInput(UserInputData &inputData)
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

void ShipUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void ShipUiHandler::updateUi()
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
}

void ShipUiHandler::updateNameComponent(const bsgo::Entity & /*ship*/)
{
  //auto text = m_shipView->getPlayerShipName();
  //text = m_shipView->getPlayerTargetName().value();
  m_menus[NAME]->setText("placeholder");
}

void ShipUiHandler::updateHealthComponent(const bsgo::Entity &ship)
{
  if (!ship.exists<bsgo::HealthComponent>())
  {
    m_menus[HEALTH]->setText("Health: N/A");
    return;
  }

  std::string text{"Health: "};
  text += floatToStr(std::floor(ship.healthComp().value()), 0);
  text += "/";
  text += floatToStr(std::floor(ship.healthComp().max()), 0);
  m_menus[HEALTH]->setText(text);
}

void ShipUiHandler::updatePowerComponent(const bsgo::Entity &ship)
{
  if (!ship.exists<bsgo::PowerComponent>())
  {
    m_menus[POWER]->setText("Power: N/A");
    return;
  }

  std::string text{"Power: "};
  text += floatToStr(std::floor(ship.powerComp().value()), 0);
  text += "/";
  text += floatToStr(std::floor(ship.powerComp().max()), 0);
  m_menus[POWER]->setText(text);
}

void ShipUiHandler::updateDistanceComponent()
{
  const auto d = m_shipView->distanceToTarget();
  m_menus[DISTANCE]->setText(floatToStr(d, 1) + "m");
}

} // namespace pge
