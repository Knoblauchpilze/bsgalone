
#include "HangarUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

HangarUiHandler::HangarUiHandler(const bsgo::Views &views)
  : IUiHandler("hangar")
  , m_playerView(views.playerView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void HangarUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};
  m_menu = generateMenu(pos, dims, "", "shop", olc::DARK_BROWN, {olc::WHITE});
}

bool HangarUiHandler::processUserInput(UserInputData &inputData)
{
  const auto out = m_menu->processUserInput(inputData.controls, inputData.actions);
  return out.relevant;
}

void HangarUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void HangarUiHandler::updateUi()
{
  if (!m_playerView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeHangar();
  }

  const auto ships = m_playerView->getPlayerShips();

  for (auto id = 0u; id < ships.size(); ++id)
  {
    const auto bgColor = ships[id].active ? olc::VERY_DARK_BROWN : olc::VERY_DARK_APPLE_GREEN;
    m_ships[id]->setBackgroundColor(bgColor);
  }
}

void HangarUiHandler::initializeHangar()
{
  initializeLayout();
  generateShipsMenus();
  m_initialized = true;
}

void HangarUiHandler::initializeLayout()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto ships = m_playerView->getPlayerShips();
  for (auto id = 0u; id < ships.size(); ++id)
  {
    auto text     = "base_ship" + std::to_string(id);
    auto bgColor  = ships[id].active ? olc::VERY_DARK_BROWN : olc::VERY_DARK_APPLE_GREEN;
    auto shipMenu = generateSlotMenu(pos, dims, "", text, bgColor, {olc::WHITE}, true);
    m_menu->addMenu(shipMenu);
    m_ships.push_back(shipMenu);
  }
}

void HangarUiHandler::generateShipsMenus()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto ships = m_playerView->getPlayerShips();

  auto id = 0;
  for (const auto &ship : ships)
  {
    auto name = generateMenu(pos, dims, ship.name, "name", olc::BLANK);
    m_ships[id]->addMenu(name);

    auto text       = "Hull points: " + floatToStr(std::floor(ship.maxHullPoints), 0);
    auto hullPoints = generateMenu(pos, dims, text, "hull_points", olc::BLANK);
    m_ships[id]->addMenu(hullPoints);

    text                 = "Hull points regen: " + floatToStr(std::floor(ship.hullPointsRegen), 0);
    auto hullPointsRegen = generateMenu(pos, dims, text, "hull_regen", olc::BLANK);
    m_ships[id]->addMenu(hullPointsRegen);

    text             = "Power points: " + floatToStr(std::floor(ship.maxPowerPoints), 0);
    auto powerPoints = generateMenu(pos, dims, text, "power_points", olc::BLANK);
    m_ships[id]->addMenu(powerPoints);

    text                  = "Power regen: " + floatToStr(std::floor(ship.powerRegen), 0);
    auto powerPointsRegen = generateMenu(pos, dims, text, "power_regen", olc::BLANK);
    m_ships[id]->addMenu(powerPointsRegen);

    text = "";
    for (const auto &[slot, count] : ship.slots)
    {
      if (!text.empty())
      {
        text += ", ";
      }
      text += std::to_string(count) + " " + bsgo::str(slot);
    }
    auto slots = generateMenu(pos, dims, text, "slots", olc::BLANK);
    m_ships[id]->addMenu(slots);

    ++id;
  }
}

} // namespace pge
