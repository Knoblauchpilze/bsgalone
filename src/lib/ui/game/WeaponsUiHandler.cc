
#include "WeaponsUiHandler.hh"
#include "ScreenCommon.hh"
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
    updateWeaponMenu(*weapon, id, ship);
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

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void WeaponsUiHandler::updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon,
                                        const int id,
                                        const bsgo::Entity &ship)
{
  const auto target = m_shipView->getPlayerTarget();

  auto &menu = *m_weapons[id];

  menu.setEnabled(target.has_value());

  auto bgColor = olc::DARK_GREEN;
  if (!weapon.active())
  {
    bgColor = olc::DARK_GREY;
  }
  else if (!target || weapon.range() < m_shipView->distanceToTarget())
  {
    bgColor = olc::DARK_RED;
  }
  else if (ship.powerComp().value() < weapon.powerCost())
  {
    bgColor = olc::DARK_ORANGE;
  }
  else if (!weapon.canFire())
  {
    bgColor = colorGradient(olc::DARK_YELLOW,
                            olc::DARK_GREEN,
                            weapon.reloadPercentage(),
                            alpha::Opaque);
  }
  else
  {
    bgColor = olc::DARK_GREEN;
  }

  menu.setBackgroundColor(bgColor);

  auto &range = *m_ranges[id];
  range.setText(floatToStr(weapon.range(), 0) + "m");

  auto &minDamage = *m_damages[2 * id];
  minDamage.setText(floatToStr(weapon.minDamage()));
  auto &maxDamage = *m_damages[2 * id + 1];
  maxDamage.setText(floatToStr(weapon.maxDamage()));
}

} // namespace pge
