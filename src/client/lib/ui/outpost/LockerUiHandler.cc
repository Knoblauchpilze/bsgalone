
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

LockerUiHandler::LockerUiHandler(const Views &views)
  : IUiHandler("locker")
  , m_playerView(views.playerView)
  , m_shipDbView(views.shipDbView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
}

void LockerUiHandler::initializeMenus(const int width,
                                      const int height,
                                      sprites::TexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  constexpr auto RESOURCES_MENU_HEIGHT = 30;
  constexpr auto HEADER_MENU_HEIGHT    = 20;

  if (height < (RESOURCES_MENU_HEIGHT + HEADER_MENU_HEIGHT))
  {
    error("Failed to generate locker ui",
          "At least " + std::to_string(RESOURCES_MENU_HEIGHT + HEADER_MENU_HEIGHT)
            + " vertical pixel(s) needed");
  }

  Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
            height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  Vec2i dims{viewWidth, RESOURCES_MENU_HEIGHT};

  m_resourcesMenu = generateBlankHorizontalMenu(pos, dims);

  pos.y += RESOURCES_MENU_HEIGHT;
  dims.y       = HEADER_MENU_HEIGHT;
  m_headerMenu = generateBlankHorizontalMenu(pos, dims);

  pos.y += HEADER_MENU_HEIGHT;
  dims.y = viewHeight - RESOURCES_MENU_HEIGHT - HEADER_MENU_HEIGHT;
  m_menu = generateBlankHorizontalMenu(pos, dims);

  auto locker = generateBlankVerticalMenu(pos, dims);
  m_locker    = locker.get();
  m_menu->addMenu(std::move(locker));

  auto ship = generateBlankVerticalMenu(pos, dims);
  m_ship    = ship.get();
  m_menu->addMenu(std::move(ship));
}

bool LockerUiHandler::processUserInput(UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void LockerUiHandler::render(Renderer &engine) const
{
  m_resourcesMenu->render(engine);
  m_headerMenu->render(engine);
  m_menu->render(engine);
}

namespace {
void updateButtonState(UiMenu &button, const bool enable)
{
  button.setEnabled(enable);

  if (!enable)
  {
    button.updateBgColor(colors::VERY_DARK_GREY);
  }
  else
  {
    button.updateBgColor(colors::VERY_DARK_GREEN);
  }
}
} // namespace

void LockerUiHandler::updateUi()
{
  if (!m_playerView->isReady() || !m_shipDbView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeLocker();
  }

  for (const auto data : m_lockerItemsData)
  {
    const auto equipable = m_shipDbView->canStillEquipItem(data.itemType);
    updateButtonState(*data.button, equipable);
  }
}

void LockerUiHandler::reset()
{
  m_resourcesMenu->clearChildren();
  m_headerMenu->clearChildren();
  m_locker->clearChildren();
  m_ship->clearChildren();

  m_lockerWeapons.clear();
  m_lockerComputers.clear();
  m_lockerItemsData.clear();

  m_shipWeapons.clear();
  m_shipComputers.clear();

  m_initialized = false;
}

void LockerUiHandler::initializeLocker()
{
  initializeLayout();
  generateResourcesMenus();
  generateLockerWeaponsMenus();
  generateLockerComputersMenus();
  generateShipWeaponsMenus();
  generateShipComputersMenus();

  m_initialized = true;
}

void LockerUiHandler::initializeLayout()
{
  const auto faction = m_playerView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_headerMenu->updateBgColor(palette.almostOpaqueColor);

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto text = textConfigFromColor("LOCKER", colors::WHITE);
  m_headerMenu->addMenu(std::make_unique<UiTextMenu>(config, bg, text));

  text = textConfigFromColor("SHIP", colors::WHITE);
  m_headerMenu->addMenu(std::make_unique<UiTextMenu>(config, bg, text));

  initializeLockerLayout();
  initializeShipLayout();
}

void LockerUiHandler::initializeLockerLayout()
{
  const auto faction = m_playerView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_locker->updateBgColor(palette.almostOpaqueColor);

  const MenuConfig config{.propagateEventsToChildren = false};

  const auto weapons  = m_playerView->getPlayerWeapons();
  const auto bgWeapon = bgConfigFromColor(colors::BLANK);
  const MenuConfig configWeapon{.layout = MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(configWeapon, bgWeapon);
    m_lockerWeapons.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto computers  = m_playerView->getPlayerComputers();
  const auto bgComputer = bgConfigFromColor(colors::BLANK);
  const MenuConfig configComputer{.layout = MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < computers.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(configComputer, bgComputer);
    m_lockerComputers.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }
}

void LockerUiHandler::initializeShipLayout()
{
  const auto faction = m_playerView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_ship->updateBgColor(palette.almostOpaqueColor);

  const MenuConfig config{.propagateEventsToChildren = false};

  const auto slots = m_shipDbView->getPlayerShipSlots();
  if (slots.contains(bsgo::Slot::WEAPON))
  {
    const auto weaponsCount = slots.at(bsgo::Slot::WEAPON);
    const auto bg           = bgConfigFromColor(colors::BLANK);
    const MenuConfig weaponConfig{.layout = MenuLayout::HORIZONTAL};
    for (auto id = 0; id < weaponsCount; ++id)
    {
      auto menu = std::make_unique<UiMenu>(weaponConfig, bg);
      m_shipWeapons.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }

  if (slots.contains(bsgo::Slot::COMPUTER))
  {
    const auto computersCount = slots.at(bsgo::Slot::COMPUTER);
    const auto bg             = bgConfigFromColor(colors::BLANK);
    const MenuConfig computerConfig{.layout = MenuLayout::HORIZONTAL};
    for (auto id = 0; id < computersCount; ++id)
    {
      auto menu = std::make_unique<UiMenu>(computerConfig, bg);
      m_shipComputers.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }
}

void LockerUiHandler::generateResourcesMenus()
{
  const auto faction = m_playerView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_resourcesMenu->updateBgColor(palette.almostOpaqueColor);

  const auto resources = m_playerView->getPlayerResources();

  const MenuConfig config{.propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  // Reverse iteration to get resources ordered according to their id.
  for (auto it = resources.rbegin(); it != resources.rend(); ++it)
  {
    auto label = textConfigFromColor(bsgo::capitalizeString(it->name, true) + ":",
                                     colors::DARK_GREY,
                                     TextAlignment::RIGHT);
    auto field = std::make_unique<UiTextMenu>(config, bg, label);
    m_resourcesMenu->addMenu(std::move(field));

    const auto amount = bsgo::floatToStr(std::floor(it->amount), 0);
    label = textConfigFromColor(amount, colorFromResourceName(it->name), TextAlignment::LEFT);
    field = std::make_unique<UiTextMenu>(config, bg, label);
    m_resourcesMenu->addMenu(std::move(field));
  }
}

void LockerUiHandler::generateLockerWeaponsMenus()
{
  auto id = 0;

  const auto weapons = m_playerView->getPlayerWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon);
    m_lockerWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection(
      "Equip", [this, itemId]() { onInstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_lockerWeapons[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId = weapon.dbId, .itemType = bsgo::Item::WEAPON, .button = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateLockerComputersMenus()
{
  auto id = 0;

  const auto computers = m_playerView->getPlayerComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer);
    m_lockerComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection(
      "Equip", [this, itemId]() { onInstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_lockerComputers[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId   = computer.dbId,
                    .itemType = bsgo::Item::COMPUTER,
                    .button   = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateShipWeaponsMenus()
{
  auto id = 0;

  const auto weapons = m_shipDbView->getPlayerShipWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon);
    m_shipWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection(
      "Remove", [this, itemId]() { onUninstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_shipWeapons[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = weapon.dbId, .itemType = bsgo::Item::WEAPON};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }

  const auto weaponSlots = static_cast<int>(
    m_shipDbView->getPlayerShipSlots().at(bsgo::Slot::WEAPON));
  for (; id < weaponSlots; ++id)
  {
    const MenuConfig config{.highlightable = false};
    const auto bg = bgConfigFromColor(colors::BLANK);

    auto text = textConfigFromColor("Empty weapon slot", colors::BLACK);
    auto prop = std::make_unique<UiTextMenu>(config, bg, text);
    m_shipWeapons[id]->addMenu(std::move(prop));
  }
}

void LockerUiHandler::generateShipComputersMenus()
{
  auto id = 0;

  const auto computers = m_shipDbView->getPlayerShipComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer);
    m_shipComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection(
      "Remove", [this, itemId]() { onUninstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_shipComputers[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = computer.dbId, .itemType = bsgo::Item::COMPUTER};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }

  const auto computerSlots = static_cast<int>(
    m_shipDbView->getPlayerShipSlots().at(bsgo::Slot::COMPUTER));
  for (; id < computerSlots; ++id)
  {
    const MenuConfig config{.highlightable = false};
    const auto bg = bgConfigFromColor(colors::BLANK);

    auto text = textConfigFromColor("Empty computer slot", colors::BLACK);
    auto prop = std::make_unique<UiTextMenu>(config, bg, text);
    m_shipComputers[id]->addMenu(std::move(prop));
  }
}

void LockerUiHandler::onInstallRequest(const int itemId)
{
  if (!m_shipDbView->isReady())
  {
    return;
  }

  const auto &equip = m_lockerItemsData.at(itemId);
  m_shipDbView->tryEquipItem(equip.itemType, equip.itemId);
}

void LockerUiHandler::onUninstallRequest(const int itemId)
{
  if (!m_shipDbView->isReady())
  {
    return;
  }

  const auto &equip = m_shipItemsData.at(itemId);
  m_shipDbView->tryUnequipItem(equip.itemType, equip.itemId);
}

} // namespace pge
