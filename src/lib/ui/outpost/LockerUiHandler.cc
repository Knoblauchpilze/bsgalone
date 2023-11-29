
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

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
  m_menu = generateMenu(pos,
                        dims,
                        "",
                        "background",
                        olc::DARK_YELLOW,
                        {olc::WHITE},
                        false,
                        false,
                        menu::Layout::Horizontal);

  m_locker = generateMenu(pos, dims, "", "locker", olc::DARK_YELLOW, {olc::WHITE});
  m_menu->addMenu(m_locker);

  m_ship = generateMenu(pos, dims, "", "ship", olc::DARK_YELLOW, {olc::WHITE});
  m_menu->addMenu(m_ship);
}

bool LockerUiHandler::processUserInput(UserInputData &inputData)
{
  const auto out = m_menu->processUserInput(inputData.controls, inputData.actions);
  return out.relevant;
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
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto resources = m_playerView->getPlayerResources();
  for (auto id = 0u; id < resources.size(); ++id)
  {
    auto text = "base_resource" + std::to_string(id);
    auto resourceMenu
      = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_GREEN, {olc::WHITE}, true);
    m_locker->addMenu(resourceMenu);
    m_resources.push_back(resourceMenu);
  }

  const auto weapons = m_playerView->getPlayerWeapons();
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto text       = "locker_weapon" + std::to_string(id);
    auto weaponMenu = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_RED, {olc::WHITE}, true);
    m_locker->addMenu(weaponMenu);
    m_lockerWeapons.push_back(weaponMenu);
  }

  const auto computers = m_playerView->getPlayerComputers();
  for (auto id = 0u; id < computers.size(); ++id)
  {
    auto text = "locker_computer" + std::to_string(id);
    auto computerMenu
      = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_YELLOW, {olc::WHITE}, true);
    m_locker->addMenu(computerMenu);
    m_lockerComputers.push_back(computerMenu);
  }
}

void LockerUiHandler::initializeShipLayout()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto slots = m_shipView->getPlayerShipSlots();

  const auto weaponsCount = slots.contains(bsgo::Slot::WEAPON) ? slots.at(bsgo::Slot::WEAPON) : 0;
  for (auto id = 0; id < weaponsCount; ++id)
  {
    auto text       = "ship_weapon" + std::to_string(id);
    auto weaponMenu = generateSlotMenu(pos, dims, "", text, olc::DARK_RED, {olc::WHITE}, true);
    m_ship->addMenu(weaponMenu);
    m_shipWeapons.push_back(weaponMenu);
  }

  const auto computersCount = slots.contains(bsgo::Slot::COMPUTER) ? slots.at(bsgo::Slot::COMPUTER)
                                                                   : 0;
  for (auto id = 0; id < computersCount; ++id)
  {
    auto text         = "ship_computer" + std::to_string(id);
    auto computerMenu = generateSlotMenu(pos, dims, "", text, olc::DARK_YELLOW, {olc::WHITE}, true);
    m_ship->addMenu(computerMenu);
    m_shipComputers.push_back(computerMenu);
  }
}

void LockerUiHandler::generateResourcesMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto resources = m_playerView->getPlayerResources();

  auto id = 0;
  for (const auto &resource : resources)
  {
    auto name = generateMenu(pos, dims, resource.name, "name", olc::BLANK);
    m_resources[id]->addMenu(name);

    const auto text = "Amount: " + floatToStr(std::floor(resource.amount), 0);
    auto amount     = generateMenu(pos, dims, text, "amount", olc::BLANK);
    m_resources[id]->addMenu(amount);

    ++id;
  }
}

void LockerUiHandler::generateLockerWeaponsMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto weapons = m_playerView->getPlayerWeapons();

  auto id = 0;
  for (const auto &weapon : weapons)
  {
    auto name = generateMenu(pos, dims, weapon.name, "name", olc::BLANK);
    m_lockerWeapons[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(weapon.level);
    auto level = generateMenu(pos, dims, text, "level", olc::BLANK);
    m_lockerWeapons[id]->addMenu(level);

    text       = "Range: " + floatToStr(weapon.range, 0) + "m";
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", olc::BLANK);
    m_lockerWeapons[id]->addMenu(range);

    ++id;
  }
}

void LockerUiHandler::generateLockerComputersMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto computers = m_playerView->getPlayerComputers();

  auto id = 0;

  for (const auto &computer : computers)
  {
    auto name = generateMenu(pos, dims, computer.name, "name", olc::BLANK);
    m_lockerComputers[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(computer.level);
    auto level = generateMenu(pos, dims, text, "level", olc::BLANK);
    m_lockerComputers[id]->addMenu(level);

    if (computer.range)
    {
      text = "Range: " + floatToStr(*computer.range, 0) + "m";
    }
    else
    {
      text = "Area";
    }
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", olc::BLANK);
    m_lockerComputers[id]->addMenu(range);

    ++id;
  }
}

void LockerUiHandler::generateShipWeaponsMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto weapons = m_shipView->getPlayerShipWeapons();

  auto id = 0;
  for (const auto &weapon : weapons)
  {
    auto name = generateMenu(pos, dims, weapon.name, "name", olc::BLANK);
    m_shipWeapons[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(weapon.level);
    auto level = generateMenu(pos, dims, text, "level", olc::BLANK);
    m_shipWeapons[id]->addMenu(level);

    text       = "Range: " + floatToStr(weapon.range, 0) + "m";
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", olc::BLANK);
    m_shipWeapons[id]->addMenu(range);

    ++id;
  }
}

void LockerUiHandler::generateShipComputersMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto computers = m_shipView->getPlayerShipComputers();

  auto id = 0;

  for (const auto &computer : computers)
  {
    auto name = generateMenu(pos, dims, computer.name, "name", olc::BLANK);
    m_shipComputers[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(computer.level);
    auto level = generateMenu(pos, dims, text, "level", olc::BLANK);
    m_shipComputers[id]->addMenu(level);

    if (computer.range)
    {
      text = "Range: " + floatToStr(*computer.range, 0) + "m";
    }
    else
    {
      text = "Area";
    }
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", olc::BLANK);
    m_shipComputers[id]->addMenu(range);

    ++id;
  }
}

} // namespace pge
