
#include "GameScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
  , m_shipView(views.shipView)
  , m_targetView(views.targetView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_targetView)
  {
    throw std::invalid_argument("Expected non null target view");
  }
}

void GameScreenUiHandler::initializeMenus(const int width, const int height)
{
  m_menus.resize(MenuItem::COUNT);

  generateShipMenus(width, height);
  generateAbilityMenus(width, height);
  generateWeaponMenus(width, height);
  generateTargetMenus(width, height);
}

auto GameScreenUiHandler::processUserInput(const controls::State &c,
                                           std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  auto relevant{false};
  auto selected{false};

  for (const auto &menu : m_menus)
  {
    const auto ih = menu->processUserInput(c, actions);
    relevant      = (relevant || ih.relevant);
    selected      = (selected || ih.selected);
  }

  return menu::InputHandle{.relevant = relevant, .selected = selected};
}

void GameScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void GameScreenUiHandler::updateUi()
{
  updateShipUi();
  updateTargetUi();
}

void GameScreenUiHandler::generateShipMenus(int /*width*/, int /*height*/)
{
  olc::vi2d pos{5, 5};
  olc::vi2d dims{200, 15};
  m_menus[HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  pos            = olc::vi2d{5, 25};
  m_menus[POWER] = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});
}

void GameScreenUiHandler::generateAbilityMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - ABILITY_COUNT * (dims.x + SPACING_IN_PIXELS);
  pos.y = height - SPACING_IN_PIXELS - dims.y;

  olc::Pixel color = olc::VERY_DARK_RED;
  color.a          = alpha::SemiOpaque;

  for (auto id = 0; id < ABILITY_COUNT; ++id)
  {
    const auto text = "A" + std::to_string(id);
    const auto name = "ability_" + std::to_string(id);

    m_menus[ABILITY_0 + id] = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void GameScreenUiHandler::generateWeaponMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - WEAPON_COUNT * (dims.x + SPACING_IN_PIXELS);
  pos.y = height / 2;

  olc::Pixel color = olc::VERY_DARK_RED;
  color.a          = alpha::SemiOpaque;

  for (auto id = 0; id < WEAPON_COUNT; ++id)
  {
    const auto text = "W" + std::to_string(id);
    const auto name = "weapon_" + std::to_string(id);

    m_menus[WEAPON_0 + id] = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void GameScreenUiHandler::generateTargetMenus(int width, int /*height*/)
{
  olc::vi2d pos{width / 2, 5};
  olc::vi2d dims{200, 15};
  m_menus[TARGET_HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  constexpr auto REASONABLE_GAP = 15;
  pos.y += REASONABLE_GAP;
  m_menus[TARGET_POWER]
    = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});

  pos.y += REASONABLE_GAP;
  m_menus[TARGET_DISTANCE] = generateMenu(pos,
                                          dims,
                                          "N/A m",
                                          "distance",
                                          olc::Pixel(0, 0, 0, alpha::Transparent),
                                          {olc::WHITE});
}

void GameScreenUiHandler::updateShipUi()
{
  std::string text;

  const auto ship = m_shipView->getPlayerShip();

  text = "Health: ";
  text += floatToStr(std::floor(ship.access<bsgo::Health>().health()), 0);
  text += "/";
  text += floatToStr(ship.access<bsgo::Health>().max(), 0);
  m_menus[HEALTH]->setText(text);

  text = "Power: ";
  text += floatToStr(std::floor(ship.access<bsgo::Power>().power()), 0);
  text += "/";
  text += floatToStr(std::floor(ship.access<bsgo::Power>().max()), 0);
  m_menus[POWER]->setText(text);
}

void GameScreenUiHandler::updateTargetUi()
{
  const auto target = m_targetView->getTarget();
  m_menus[TARGET_HEALTH]->setVisible(target.has_value());
  m_menus[TARGET_POWER]->setVisible(target.has_value());
  m_menus[TARGET_DISTANCE]->setVisible(target.has_value());

  if (!target)
  {
    return;
  }

  std::string text;

  text                  = "Health: ";
  const auto healthComp = target->health;
  if (!healthComp)
  {
    text += "N/A";
  }
  else
  {
    const auto &healthPtr = *healthComp;
    text += floatToStr(std::floor(healthPtr->health()), 0);
    text += "/";
    text += floatToStr(std::floor(healthPtr->max()), 0);
  }
  m_menus[TARGET_HEALTH]->setText(text);

  text                 = "Power: ";
  const auto powerComp = target->power;
  if (!powerComp)
  {
    text += "N/A";
  }
  else
  {
    const auto &powerPtr = *powerComp;
    text += floatToStr(std::floor(powerPtr->power()), 0);
    text += "/";
    text += floatToStr(std::floor(powerPtr->max()), 0);
  }
  m_menus[TARGET_POWER]->setText(text);

  const auto d = m_targetView->distanceToTarget();
  text         = floatToStr(d, 1) + "m";
  m_menus[TARGET_DISTANCE]->setText(text);
}

} // namespace pge
