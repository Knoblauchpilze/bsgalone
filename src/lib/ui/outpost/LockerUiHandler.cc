
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

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};

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

void LockerUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

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
}

void LockerUiHandler::reset()
{
  m_locker->clearChildren();
  m_ship->clearChildren();

  m_resources.clear();
  m_lockerWeapons.clear();
  m_lockerComputers.clear();
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

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};
} // namespace

void LockerUiHandler::initializeLockerLayout()
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};

  const auto resources  = m_playerView->getPlayerResources();
  const auto bgResource = bgConfigFromColor(olc::DARK_GREEN);
  for (auto id = 0u; id < resources.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(config, bgResource);
    m_resources.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto weapons  = m_playerView->getPlayerWeapons();
  const auto bgWeapon = bgConfigFromColor(olc::DARK_RED);
  const MenuConfig configWeapon{.pos    = {},
                                .dims   = DUMMY_DIMENSION,
                                .layout = MenuLayout::HORIZONTAL};
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(configWeapon, bgWeapon);
    m_lockerWeapons.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }

  const auto computers  = m_playerView->getPlayerComputers();
  const auto bgComputer = bgConfigFromColor(olc::DARK_YELLOW);
  for (auto id = 0u; id < computers.size(); ++id)
  {
    auto menu = std::make_unique<UiMenu>(config, bgComputer);
    m_lockerComputers.push_back(menu.get());
    m_locker->addMenu(std::move(menu));
  }
}

void LockerUiHandler::initializeShipLayout()
{
  const auto slots = m_shipView->getPlayerShipSlots();

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};

  if (slots.contains(bsgo::Slot::WEAPON))
  {
    const auto weaponsCount = slots.at(bsgo::Slot::WEAPON);
    const auto bg           = bgConfigFromColor(olc::DARK_RED);
    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .layout = MenuLayout::HORIZONTAL};
    for (auto id = 0; id < weaponsCount; ++id)
    {
      auto menu = std::make_unique<UiMenu>(config, bg);
      m_shipWeapons.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }

  if (slots.contains(bsgo::Slot::COMPUTER))
  {
    const auto computersCount = slots.at(bsgo::Slot::COMPUTER);
    const auto bg             = bgConfigFromColor(olc::DARK_YELLOW);
    for (auto id = 0; id < computersCount; ++id)
    {
      auto menu = std::make_unique<UiMenu>(config, bg);
      m_shipComputers.push_back(menu.get());
      m_ship->addMenu(std::move(menu));
    }
  }
}

void LockerUiHandler::generateResourcesMenus()
{
  const auto resources = m_playerView->getPlayerResources();

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(olc::BLANK);
  auto id       = 0;

  for (const auto &resource : resources)
  {
    auto textConf = textConfigFromColor(resource.name, olc::BLACK);
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

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .highlightable = false};
  const auto bg = bgConfigFromColor(olc::BLANK);

  auto textConf = textConfigFromColor(weapon.name, olc::BLACK);
  auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  textConf.text = "Level: " + std::to_string(weapon.level);
  field         = std::make_unique<UiTextMenu>(config, bg, textConf);
  menu->addMenu(std::move(field));

  return menu;
}

auto generateInteractiveSection(const std::string &buttonText, const ClickCallback &callback)
  -> UiMenuPtr
{
  auto middleSection = generateBlankVerticalMenu();
  middleSection->addMenu(generateSpacer());

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .clickCallback = callback};

  const auto bg       = bgConfigFromColor(olc::VERY_DARK_GREEN);
  const auto textConf = textConfigFromColor(buttonText, olc::WHITE, olc::WHITE);
  auto button         = std::make_unique<UiTextMenu>(config, bg, textConf);

  middleSection->addMenu(std::move(button));

  middleSection->addMenu(generateSpacer());

  auto menu = generateBlankHorizontalMenu();
  menu->addMenu(std::move(middleSection));
  menu->addMenu(generateSpacer());

  return menu;
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
    auto install = generateInteractiveSection("Equip", [this, id]() {
      onInstallRequest(bsgo::Item::WEAPON, id);
    });
    m_lockerWeapons[id]->addMenu(std::move(install));

    ++id;
  }
}

void LockerUiHandler::generateLockerComputersMenus()
{
  const auto computers = m_playerView->getPlayerComputers();

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(olc::BLANK);
  auto id       = 0;

  for (const auto &computer : computers)
  {
    auto textConf = textConfigFromColor(computer.name, olc::BLACK);
    auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_lockerComputers[id]->addMenu(std::move(field));

    textConf.text = "Level: " + std::to_string(computer.level);
    field         = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_lockerComputers[id]->addMenu(std::move(field));

    if (computer.range)
    {
      textConf.text = "Range: " + floatToStr(*computer.range, 0) + "m";
    }
    else
    {
      textConf.text = "Area";
    }
    field = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_lockerComputers[id]->addMenu(std::move(field));

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
    auto install = generateInteractiveSection("Remove", [this, id]() {
      onUninstallRequest(bsgo::Item::WEAPON, id);
    });
    m_shipWeapons[id]->addMenu(std::move(install));

    ++id;
  }
}

void LockerUiHandler::generateShipComputersMenus()
{
  const auto computers = m_shipView->getPlayerShipComputers();

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(olc::BLANK);
  auto id       = 0;

  for (const auto &computer : computers)
  {
    auto textConf = textConfigFromColor(computer.name, olc::BLACK);
    auto field    = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_shipComputers[id]->addMenu(std::move(field));

    textConf.text = "Level: " + std::to_string(computer.level);
    field         = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_shipComputers[id]->addMenu(std::move(field));

    if (computer.range)
    {
      textConf.text = "Range: " + floatToStr(*computer.range, 0) + "m";
    }
    else
    {
      textConf.text = "Area";
    }
    field = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_shipComputers[id]->addMenu(std::move(field));

    ++id;
  }
}

void LockerUiHandler::onInstallRequest(const bsgo::Item &type, const int id)
{
  warn("Should process install: " + bsgo::str(type) + ", " + std::to_string(id));
}

void LockerUiHandler::onUninstallRequest(const bsgo::Item &type, const int id)
{
  warn("Should process uninstall: " + bsgo::str(type) + ", " + std::to_string(id));
}

} // namespace pge
