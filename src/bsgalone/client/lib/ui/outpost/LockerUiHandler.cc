
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

LockerUiHandler::LockerUiHandler(const Views &views)
  : AbstractUiHandler("locker")
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }

  subscribeToViews();
}

void LockerUiHandler::initializeMenus(const int width,
                                      const int height,
                                      pge::sprites::TexturePack & /*texturesLoader*/)
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

  pge::Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                 height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  pge::Vec2i dims{viewWidth, RESOURCES_MENU_HEIGHT};

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

bool LockerUiHandler::processUserInput(ui::UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void LockerUiHandler::render(pge::Renderer &engine) const
{
  m_resourcesMenu->render(engine);
  m_headerMenu->render(engine);
  m_menu->render(engine);
}

namespace {
void updateButtonState(ui::UiMenu &button, const bool enable)
{
  button.setEnabled(enable);

  if (!enable)
  {
    button.updateBgColor(pge::colors::VERY_DARK_GREY);
  }
  else
  {
    button.updateBgColor(pge::colors::VERY_DARK_GREEN);
  }
}
} // namespace

void LockerUiHandler::updateUi()
{
  if (!m_gameView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeLocker();
  }

  for (const auto data : m_lockerItemsData)
  {
    const auto equipable = m_gameView->canStillEquipItem(data.itemType);
    updateButtonState(*data.button, equipable);
  }
}

void LockerUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_gameView->addListener(std::move(listener));
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
  const auto faction = m_gameView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_headerMenu->updateBgColor(palette.almostOpaqueColor);

  const ui::MenuConfig config{.highlightable = false};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  auto text = ui::textConfigFromColor("LOCKER", pge::colors::WHITE);
  m_headerMenu->addMenu(std::make_unique<ui::UiTextMenu>(config, bg, text));

  text = ui::textConfigFromColor("SHIP", pge::colors::WHITE);
  m_headerMenu->addMenu(std::make_unique<ui::UiTextMenu>(config, bg, text));

  initializeLockerLayout();
  initializeShipLayout();
}

void LockerUiHandler::initializeLockerLayout()
{
  const auto faction = m_gameView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_locker->updateBgColor(palette.almostOpaqueColor);

  const ui::MenuConfig config{.propagateEventsToChildren = false};

  const auto weapons  = m_gameView->getPlayerWeapons();
  const auto bgWeapon = ui::bgConfigFromColor(pge::colors::BLANK);
  const ui::MenuConfig configWeapon{.layout = ui::MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto menu = std::make_unique<ui::UiMenu>(configWeapon, bgWeapon);
    m_lockerWeapons.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto computers  = m_gameView->getPlayerComputers();
  const auto bgComputer = ui::bgConfigFromColor(pge::colors::BLANK);
  const ui::MenuConfig configComputer{.layout = ui::MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < computers.size(); ++id)
  {
    auto menu = std::make_unique<ui::UiMenu>(configComputer, bgComputer);
    m_lockerComputers.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }
}

void LockerUiHandler::initializeShipLayout()
{
  const auto faction = m_gameView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_ship->updateBgColor(palette.almostOpaqueColor);

  const ui::MenuConfig config{.propagateEventsToChildren = false};

  const auto slots = m_gameView->getPlayerShipSlots();
  if (slots.contains(core::Slot::WEAPON))
  {
    const auto weaponsCount = slots.at(core::Slot::WEAPON);
    const auto bg           = ui::bgConfigFromColor(pge::colors::BLANK);
    const ui::MenuConfig weaponConfig{.layout = ui::MenuLayout::HORIZONTAL};
    for (auto id = 0; id < weaponsCount; ++id)
    {
      auto menu = std::make_unique<ui::UiMenu>(weaponConfig, bg);
      m_shipWeapons.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }

  if (slots.contains(core::Slot::COMPUTER))
  {
    const auto computersCount = slots.at(core::Slot::COMPUTER);
    const auto bg             = ui::bgConfigFromColor(pge::colors::BLANK);
    const ui::MenuConfig computerConfig{.layout = ui::MenuLayout::HORIZONTAL};
    for (auto id = 0; id < computersCount; ++id)
    {
      auto menu = std::make_unique<ui::UiMenu>(computerConfig, bg);
      m_shipComputers.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }
}

void LockerUiHandler::generateResourcesMenus()
{
  const auto faction = m_gameView->getPlayerFaction();
  const auto palette = generatePaletteForFaction(faction);

  m_resourcesMenu->updateBgColor(palette.almostOpaqueColor);

  const auto resources = m_gameView->getPlayerResources();

  const ui::MenuConfig config{.propagateEventsToChildren = false};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  // Reverse iteration to get resources ordered according to their id.
  for (auto it = resources.rbegin(); it != resources.rend(); ++it)
  {
    auto label = ui::textConfigFromColor(core::capitalizeString(it->name, true) + ":",
                                         pge::colors::DARK_GREY,
                                         ui::TextAlignment::RIGHT);
    auto field = std::make_unique<ui::UiTextMenu>(config, bg, label);
    m_resourcesMenu->addMenu(std::move(field));

    const auto amount = std::to_string(it->amount);
    label             = ui::textConfigFromColor(amount,
                                    colorFromResourceName(it->name),
                                    ui::TextAlignment::LEFT);
    field             = std::make_unique<ui::UiTextMenu>(config, bg, label);
    m_resourcesMenu->addMenu(std::move(field));
  }
}

void LockerUiHandler::generateLockerWeaponsMenus()
{
  const auto &gameSession = m_gameView->gameSession();

  auto id            = 0;
  const auto weapons = m_gameView->getPlayerWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon, gameSession.getTimeStep());
    m_lockerWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection(
      "Equip", [this, itemId]() { onInstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_lockerWeapons[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId = weapon.dbId, .itemType = core::Item::WEAPON, .button = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateLockerComputersMenus()
{
  const auto &gameSession = m_gameView->gameSession();

  auto id              = 0;
  const auto computers = m_gameView->getPlayerComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer, gameSession.getTimeStep());
    m_lockerComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection(
      "Equip", [this, itemId]() { onInstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_lockerComputers[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId   = computer.dbId,
                    .itemType = core::Item::COMPUTER,
                    .button   = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateShipWeaponsMenus()
{
  const auto &gameSession = m_gameView->gameSession();

  auto id            = 0;
  const auto weapons = m_gameView->getPlayerShipWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon, gameSession.getTimeStep());
    m_shipWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection(
      "Remove", [this, itemId]() { onUninstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_shipWeapons[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = weapon.dbId, .itemType = core::Item::WEAPON};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }

  const auto weaponSlots = static_cast<int>(m_gameView->getPlayerShipSlots().at(core::Slot::WEAPON));
  for (; id < weaponSlots; ++id)
  {
    const ui::MenuConfig config{.highlightable = false};
    const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

    auto text = ui::textConfigFromColor("Empty weapon slot", pge::colors::BLACK);
    auto prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
    m_shipWeapons[id]->addMenu(std::move(prop));
  }
}

void LockerUiHandler::generateShipComputersMenus()
{
  const auto &gameSession = m_gameView->gameSession();

  auto id              = 0;
  const auto computers = m_gameView->getPlayerShipComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer, gameSession.getTimeStep());
    m_shipComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection(
      "Remove", [this, itemId]() { onUninstallRequest(itemId); }, HorizontalMargin::LEFT);
    m_shipComputers[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = computer.dbId, .itemType = core::Item::COMPUTER};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }

  const auto computerSlots = static_cast<int>(
    m_gameView->getPlayerShipSlots().at(core::Slot::COMPUTER));
  for (; id < computerSlots; ++id)
  {
    const ui::MenuConfig config{.highlightable = false};
    const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

    auto text = ui::textConfigFromColor("Empty computer slot", pge::colors::BLACK);
    auto prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
    m_shipComputers[id]->addMenu(std::move(prop));
  }
}

void LockerUiHandler::onInstallRequest(const int itemId)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  const auto &equip = m_lockerItemsData.at(itemId);
  m_gameView->tryEquipItem(equip.itemType, equip.itemId);
}

void LockerUiHandler::onUninstallRequest(const int itemId)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  const auto &equip = m_shipItemsData.at(itemId);
  m_gameView->tryUnequipItem(equip.itemType, equip.itemId);
}

} // namespace bsgalone::client
