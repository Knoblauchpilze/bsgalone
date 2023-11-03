
#include "LockerUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

LockerUiHandler::LockerUiHandler(const bsgo::Views &views)
  : IUiHandler("locker")
  , m_playerView(views.playerView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void LockerUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menu = generateMenu(pos, dims, "", "locker", olc::DARK_YELLOW, {olc::WHITE});

  initializeLayout();
  generateResourcesMenus();
  generateWeaponsMenus();
  generateComputersMenus();
}

auto LockerUiHandler::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void LockerUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void LockerUiHandler::updateUi() {}

void LockerUiHandler::initializeLayout()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto resources = m_playerView->getPlayerResources();
  for (auto id = 0u; id < resources.size(); ++id)
  {
    auto text = "base_resource" + std::to_string(id);
    auto resourceMenu
      = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_GREEN, {olc::WHITE}, true);
    m_menu->addMenu(resourceMenu);
    m_resources.push_back(resourceMenu);
  }

  const auto weapons = m_playerView->getPlayerWeapons();
  for (auto id = 0u; id < weapons.size(); ++id)
  {
    auto text       = "base_weapon" + std::to_string(id);
    auto weaponMenu = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_RED, {olc::WHITE}, true);
    m_menu->addMenu(weaponMenu);
    m_weapons.push_back(weaponMenu);
  }

  const auto computers = m_playerView->getPlayerComputers();
  for (auto id = 0u; id < computers.size(); ++id)
  {
    auto text = "base_computer" + std::to_string(id);
    auto computerMenu
      = generateSlotMenu(pos, dims, "", text, olc::VERY_DARK_YELLOW, {olc::WHITE}, true);
    m_menu->addMenu(computerMenu);
    m_computers.push_back(computerMenu);
  }
}

void LockerUiHandler::generateResourcesMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  const auto resources = m_playerView->getPlayerResources();

  auto id = 0;
  for (const auto &resource : resources)
  {
    auto name = generateMenu(pos, dims, resource.name, "name", transparentBg);
    m_resources[id]->addMenu(name);

    const auto text = "Amount: " + floatToStr(std::floor(resource.amount), 0);
    auto amount     = generateMenu(pos, dims, text, "amount", transparentBg);
    m_resources[id]->addMenu(amount);

    ++id;
  }
}

void LockerUiHandler::generateWeaponsMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  const auto weapons = m_playerView->getPlayerWeapons();

  auto id = 0;
  for (const auto &weapon : weapons)
  {
    auto name = generateMenu(pos, dims, weapon.name, "name", transparentBg);
    m_weapons[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(weapon.level);
    auto level = generateMenu(pos, dims, text, "level", transparentBg);
    m_weapons[id]->addMenu(level);

    text       = "Range: " + floatToStr(weapon.range, 0) + "m";
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", transparentBg);
    m_weapons[id]->addMenu(range);

    ++id;
  }
}

void LockerUiHandler::generateComputersMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  const auto computers = m_playerView->getPlayerComputers();

  auto id = 0;

  for (const auto &computer : computers)
  {
    auto name = generateMenu(pos, dims, computer.name, "name", transparentBg);
    m_computers[id]->addMenu(name);

    auto text  = "Level: " + std::to_string(computer.level);
    auto level = generateMenu(pos, dims, text, "level", transparentBg);
    m_computers[id]->addMenu(level);

    if (computer.range)
    {
      text = "Range: " + floatToStr(*computer.range, 0) + "m";
    }
    else
    {
      text = "Area";
    }
    auto range = generateMenu(olc::vi2d{}, dims, text, "range", transparentBg);
    m_computers[id]->addMenu(range);

    ++id;
  }
}

} // namespace pge
