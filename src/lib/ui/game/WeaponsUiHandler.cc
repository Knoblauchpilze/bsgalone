
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

auto WeaponsUiHandler::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  auto relevant{false};
  auto selected{false};

  for (const auto &menu : m_weapons)
  {
    const auto ih = menu->processUserInput(c, actions);
    relevant      = (relevant || ih.relevant);
    selected      = (selected || ih.selected);
  }

  return menu::InputHandle{.relevant = relevant, .selected = selected};
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
  const auto ship = m_shipView->getPlayerShip();

  auto id = 0;
  for (const auto &weapon : ship.weapons)
  {
    updateWeaponMenu(*weapon, id);
    ++id;
  }
}

constexpr auto REASONABLE_MAXIMUM_NUMBER_OF_WEAPONS = 4;

void WeaponsUiHandler::generateWeaponsMenus(int width, int height)
{
  const auto ship         = m_shipView->getPlayerShip();
  const auto weaponsCount = ship.weapons.size();
  if (weaponsCount > REASONABLE_MAXIMUM_NUMBER_OF_WEAPONS)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(REASONABLE_MAXIMUM_NUMBER_OF_WEAPONS)
            + " weapon(s), got " + std::to_string(ship.weapons.size()));
  }

  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - weaponsCount * (dims.x + SPACING_IN_PIXELS);
  pos.y = height / 2;

  olc::Pixel color = olc::Pixel{0, 0, 0, alpha::Transparent};
  color.a          = alpha::SemiOpaque;

  for (auto id = 0u; id < weaponsCount; ++id)
  {
    const auto text = "W" + std::to_string(id);
    const auto name = "weapon_" + std::to_string(id);

    auto menu = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);
    m_weapons.push_back(menu);

    const auto range = generateMenu(pos, dims, "range", "range", color, {olc::WHITE});
    menu->addMenu(range);
    m_ranges.push_back(range);
    auto damage = generateMenu(pos, dims, "min damage", "min_damage", color, {olc::WHITE});
    menu->addMenu(damage);
    m_damages.push_back(damage);
    damage = generateMenu(pos, dims, "max damage", "max_damage", color, {olc::WHITE});
    menu->addMenu(damage);
    m_damages.push_back(damage);
    const auto status = generateMenu(pos, dims, "status", "status", color, {olc::WHITE});
    menu->addMenu(status);
    m_statuses.push_back(status);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void WeaponsUiHandler::updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id)
{
  auto &menu = *m_weapons[id];
  menu.setEnabled(m_shipView->hasTarget());

  auto bgColor = bgColorFromFiringState(weapon);
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
