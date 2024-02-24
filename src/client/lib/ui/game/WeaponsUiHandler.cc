
#include "WeaponsUiHandler.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

WeaponsUiHandler::WeaponsUiHandler(const bsgo::Views &views)
  : IUiHandler("weapons")
  , AbstractMessageListener({bsgo::MessageType::ENTITY_REMOVED})
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
}

void WeaponsUiHandler::initializeMenus(const int width, const int height)
{
  generateWeaponsMenus(width, height);
}

bool WeaponsUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_disabled)
  {
    return false;
  }

  auto out{false};
  for (const auto &menu : m_weapons)
  {
    out |= menu->processUserInput(inputData);
  }

  return out;
}

void WeaponsUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_weapons)
  {
    menu->render(engine);
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

namespace {
constexpr auto NUMBER_OF_WEAPONS = 6;
} // namespace

void WeaponsUiHandler::reset()
{
  m_ranges.clear();
  m_damages.clear();
  m_statuses.clear();

  for (auto &weapon : m_weapons)
  {
    weapon->clearChildren();
    weapon->updateBgColor(semiOpaque(colors::BLACK));
  }

  m_initialized = false;
  m_disabled    = false;
}

void WeaponsUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void WeaponsUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  m_disabled = didPlayerShipDied(message.as<bsgo::EntityRemovedMessage>(), *m_shipDbView);
}

void WeaponsUiHandler::generateWeaponsMenus(int width, int height)
{
  constexpr auto WEAPON_MENU_PIXEL_DIMS = 50;
  const Vec2i weaponMenuDims{WEAPON_MENU_PIXEL_DIMS, WEAPON_MENU_PIXEL_DIMS};
  constexpr auto SPACING_IN_PIXELS = 5;
  const Vec2i pos{width - NUMBER_OF_WEAPONS * (weaponMenuDims.x + SPACING_IN_PIXELS), height / 2};

  MenuConfig config{.pos = pos, .dims = weaponMenuDims, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(semiOpaque(colors::BLACK));

  for (auto id = 0u; id < NUMBER_OF_WEAPONS; ++id)
  {
    auto menu = std::make_unique<UiMenu>(config, bg);
    m_weapons.push_back(std::move(menu));

    config.pos.x += (weaponMenuDims.x + SPACING_IN_PIXELS);
  }
}

void WeaponsUiHandler::initializeWeapons()
{
  const auto ship         = m_shipView->getPlayerShip();
  const auto weaponsCount = ship.weapons.size();
  if (weaponsCount > NUMBER_OF_WEAPONS)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_WEAPONS) + " weapon(s), got "
            + std::to_string(ship.weapons.size()));
  }

  const MenuConfig config{};
  const auto bg = bgConfigFromColor(colors::BLANK);
  auto textConf = textConfigFromColor("", colors::WHITE);

  for (auto id = 0u; id < weaponsCount; ++id)
  {
    auto &menu = m_weapons[id];

    menu->setClickCallback([this, id]() {
      if (!m_shipView->isReady())
      {
        return;
      }
      m_shipView->tryActivateWeapon(id);
    });
    menu->setEnabled(true);

    auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ranges.push_back(prop.get());
    menu->addMenu(std::move(prop));

    prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_damages.push_back(prop.get());
    menu->addMenu(std::move(prop));

    prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_damages.push_back(prop.get());
    menu->addMenu(std::move(prop));

    prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));
  }

  m_initialized = true;
}

void WeaponsUiHandler::updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id)
{
  auto &menu = *m_weapons[id];

  auto bgColor = semiOpaque(bgColorFromFiringState(weapon));
  menu.updateBgColor(bgColor);

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
