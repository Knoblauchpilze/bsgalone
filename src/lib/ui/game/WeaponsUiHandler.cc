
#include "WeaponsUiHandler.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

WeaponsUiHandler::WeaponsUiHandler(const bsgo::Views &views)
  : IUiHandler("weapons")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void WeaponsUiHandler::initializeMenus(const int width, const int height)
{
  generateWeaponsMenus(width, height);
}

bool WeaponsUiHandler::processUserInput(UserInputData &inputData)
{
  auto relevant{false};

  for (const auto &menu : m_weapons)
  {
    const auto ih = menu->processUserInput(inputData.controls, inputData.actions);
    relevant      = (relevant || ih.relevant);
  }

  return relevant;
}

void WeaponsUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_weapons)
  {
    menu->render(engine.getRenderer());
  }
}

void WeaponsUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeWeapons();
  }

  const auto ship = m_shipView->getPlayerShip();

  auto id = 0;
  for (const auto &weapon : ship.weapons)
  {
    updateWeaponMenu(*weapon, id);
    ++id;
  }
}

constexpr auto NUMBER_OF_WEAPONS = 4;

void WeaponsUiHandler::generateWeaponsMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - NUMBER_OF_WEAPONS * (dims.x + SPACING_IN_PIXELS);
  pos.y = height / 2;
  const olc::Pixel transparentBg{0, 0, 0, alpha::SemiOpaque};

  for (auto id = 0u; id < NUMBER_OF_WEAPONS; ++id)
  {
    const auto name = "weapon_" + std::to_string(id);
    auto menu       = generateSlotMenu(pos, dims, "", name, transparentBg, {olc::WHITE}, true);
    m_weapons.push_back(menu);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void WeaponsUiHandler::initializeWeapons()
{
  const auto ship         = m_shipView->getPlayerShip();
  const auto shipId       = ship.uuid;
  const auto weaponsCount = ship.weapons.size();
  if (weaponsCount > NUMBER_OF_WEAPONS)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_WEAPONS) + " weapon(s), got "
            + std::to_string(ship.weapons.size()));
  }

  const olc::vi2d dummyPos{0, 0};
  const olc::vi2d dummyDims{10, 10};
  const olc::Pixel transparentBg{0, 0, 0, alpha::SemiOpaque};
  for (auto id = 0u; id < weaponsCount; ++id)
  {
    auto &menu = m_weapons[id];

    menu->setSimpleAction([shipId, id](Game &g) { g.tryActivateWeapon(shipId, id); });
    menu->setEnabled(true);

    const auto range
      = generateMenu(dummyPos, dummyDims, "range", "range", transparentBg, {olc::WHITE});
    menu->addMenu(range);
    m_ranges.push_back(range);
    auto damage
      = generateMenu(dummyPos, dummyDims, "min damage", "min_damage", transparentBg, {olc::WHITE});
    menu->addMenu(damage);
    m_damages.push_back(damage);
    damage
      = generateMenu(dummyPos, dummyDims, "max damage", "max_damage", transparentBg, {olc::WHITE});
    menu->addMenu(damage);
    m_damages.push_back(damage);
    const auto status
      = generateMenu(dummyPos, dummyDims, "status", "status", transparentBg, {olc::WHITE});
    menu->addMenu(status);
    m_statuses.push_back(status);
  }

  m_initialized = true;
}

void WeaponsUiHandler::updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id)
{
  auto &menu = *m_weapons[id];

  auto bgColor = bgColorFromFiringState(weapon);
  bgColor.a    = alpha::SemiOpaque;
  menu.setBackgroundColor(bgColor);

  auto &range = *m_ranges[id];
  range.setText(floatToStr(weapon.range(), 0) + "m");

  auto &minDamage = *m_damages[2 * id];
  minDamage.setText(floatToStr(weapon.minDamage()));
  auto &maxDamage = *m_damages[2 * id + 1];
  maxDamage.setText(floatToStr(weapon.maxDamage()));

  std::string statusText("ready");
  if (!weapon.active())
  {
    statusText = "off";
  }
  if (!weapon.canFire())
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = floatToStr(PERCENTAGE_MULTIPLIER * weapon.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace pge
