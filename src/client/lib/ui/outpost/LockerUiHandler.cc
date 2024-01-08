
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

LockerUiHandler::LockerUiHandler(const bsgo::Views &views)
  : IUiHandler("locker")
  , m_playerView(views.playerView)
  , m_shipView(views.shipView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void LockerUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                  height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const Vec2i dims{viewWidth, viewHeight};

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
  return m_menu->processUserInput(inputData);
}

void LockerUiHandler::render(Renderer &engine) const
{
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
  if (!m_playerView->isReady() || !m_shipView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeLocker();
  }

  for (const auto data : m_lockerItemsData)
  {
    const auto equipable = m_shipView->canStillEquipItem(data.itemType);
    updateButtonState(*data.button, equipable);
  }
}

void LockerUiHandler::reset()
{
  m_locker->clearChildren();
  m_ship->clearChildren();

  m_resources.clear();

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
  initializeLockerLayout();
  initializeShipLayout();
}

void LockerUiHandler::initializeLockerLayout()
{
  MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};

  const auto bg   = bgConfigFromColor(colors::DARK_GREY);
  const auto text = textConfigFromColor("Locker", colors::BLACK);
  auto title      = std::make_unique<UiTextMenu>(config, bg, text);
  m_locker->addMenu(std::move(title));

  config.highlightable = true;

  const auto resources  = m_playerView->getPlayerResources();
  const auto bgResource = bgConfigFromColor(colors::DARK_GREEN);
  for (auto id = 0u; id < resources.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(config, bgResource);
    m_resources.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto weapons  = m_playerView->getPlayerWeapons();
  const auto bgWeapon = bgConfigFromColor(colors::DARK_RED);
  const MenuConfig configWeapon{.layout = MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(configWeapon, bgWeapon);
    m_lockerWeapons.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto computers  = m_playerView->getPlayerComputers();
  const auto bgComputer = bgConfigFromColor(colors::DARK_YELLOW);
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
  MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};

  const auto bg   = bgConfigFromColor(colors::DARK_GREY);
  const auto text = textConfigFromColor("Ship", colors::BLACK);
  auto title      = std::make_unique<UiTextMenu>(config, bg, text);
  m_ship->addMenu(std::move(title));

  config.highlightable = true;

  const auto slots = m_shipView->getPlayerShipSlots();
  if (slots.contains(bsgo::Slot::WEAPON))
  {
    const auto weaponsCount = slots.at(bsgo::Slot::WEAPON);
    const auto bg           = bgConfigFromColor(colors::DARK_RED);
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
    const auto bg             = bgConfigFromColor(colors::DARK_YELLOW);
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
  const auto resources = m_playerView->getPlayerResources();

  const MenuConfig config{.propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(colors::BLANK);
  auto id       = 0;

  for (const auto &resource : resources)
  {
    auto textConf = textConfigFromColor(resource.name, colors::BLACK);
    auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_resources[id]->addMenu(std::move(field));

    textConf.text = "Amount: " + floatToStr(std::floor(resource.amount), 0);
    field         = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_resources[id]->addMenu(std::move(field));

    ++id;
  }
}

namespace {
auto generateWeaponMenu(const bsgo::PlayerWeapon &weapon) -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto textConf = textConfigFromColor(weapon.name, colors::BLACK);
  auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  textConf.text = "Level: " + std::to_string(weapon.level);
  field         = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  return menu;
}

struct InteractiveSection
{
  UiMenuPtr menu{};
  UiMenu *button{};
};

auto generateInteractiveSection(const std::string &buttonText, const ClickCallback &callback)
  -> InteractiveSection
{
  InteractiveSection section{};

  auto middleSection = generateBlankVerticalMenu();
  middleSection->addMenu(generateSpacer());

  const MenuConfig config{.clickCallback = callback};

  const auto bg       = bgConfigFromColor(colors::VERY_DARK_GREEN);
  const auto textConf = textConfigFromColor(buttonText, colors::WHITE, colors::WHITE);
  auto button         = std::make_unique<UiTextMenu>(config, bg, textConf);
  section.button      = button.get();

  middleSection->addMenu(std::move(button));

  middleSection->addMenu(generateSpacer());

  section.menu = generateBlankHorizontalMenu();
  section.menu->addMenu(std::move(middleSection));
  section.menu->addMenu(generateSpacer());

  return section;
}
} // namespace

void LockerUiHandler::generateLockerWeaponsMenus()
{
  auto id = 0;

  const auto weapons = m_playerView->getPlayerWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon);
    m_lockerWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection("Equip",
                                              [this, itemId]() { onInstallRequest(itemId); });
    m_lockerWeapons[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId = weapon.id, .itemType = bsgo::Item::WEAPON, .button = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

namespace {
auto generateComputerMenu(const bsgo::PlayerComputer &computer) -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto textConf = textConfigFromColor(computer.name, colors::BLACK);
  auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  textConf.text = "Level: " + std::to_string(computer.level);
  field         = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  return menu;
}
} // namespace

void LockerUiHandler::generateLockerComputersMenus()
{
  auto id = 0;

  const auto computers = m_playerView->getPlayerComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer);
    m_lockerComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_lockerItemsData.size());
    auto section      = generateInteractiveSection("Equip",
                                              [this, itemId]() { onInstallRequest(itemId); });
    m_lockerComputers[id]->addMenu(std::move(section.menu));

    LockerItem data{.itemId   = computer.id,
                    .itemType = bsgo::Item::COMPUTER,
                    .button   = section.button};
    m_lockerItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateShipWeaponsMenus()
{
  auto id = 0;

  const auto weapons = m_shipView->getPlayerShipWeapons();
  for (const auto &weapon : weapons)
  {
    auto details = generateWeaponMenu(weapon);
    m_shipWeapons[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection("Remove",
                                              [this, itemId]() { onUninstallRequest(itemId); });
    m_shipWeapons[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = weapon.id, .itemType = bsgo::Item::WEAPON};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::generateShipComputersMenus()
{
  auto id = 0;

  const auto computers = m_shipView->getPlayerShipComputers();
  for (const auto &computer : computers)
  {
    auto details = generateComputerMenu(computer);
    m_shipComputers[id]->addMenu(std::move(details));

    const auto itemId = static_cast<int>(m_shipItemsData.size());
    auto section      = generateInteractiveSection("Remove",
                                              [this, itemId]() { onUninstallRequest(itemId); });
    m_shipComputers[id]->addMenu(std::move(section.menu));

    ShipItem data{.itemId = computer.id, .itemType = bsgo::Item::COMPUTER};
    m_shipItemsData.emplace_back(std::move(data));

    ++id;
  }
}

void LockerUiHandler::onInstallRequest(const int itemId)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto &equip = m_lockerItemsData.at(itemId);
  m_shipView->tryEquipItem(equip.itemType, equip.itemId);
}

void LockerUiHandler::onUninstallRequest(const int itemId)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const auto &equip = m_shipItemsData.at(itemId);
  m_shipView->tryUnequipItem(equip.itemType, equip.itemId);
}

} // namespace pge
