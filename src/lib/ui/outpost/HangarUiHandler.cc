
#include "HangarUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

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
  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool HangarUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
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
    m_ships[id]->updateBgColor(bgColor);
  }
}

void HangarUiHandler::initializeHangar()
{
  initializeLayout();
  generateShipsMenus();
  m_initialized = true;
}

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};
} // namespace

void HangarUiHandler::initializeLayout()
{
  const auto ships = m_playerView->getPlayerShips();
  for (auto id = 0u; id < ships.size(); ++id)
  {
    auto bgColor = ships[id].active ? olc::VERY_DARK_BROWN : olc::VERY_DARK_APPLE_GREEN;

    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
    const auto bg = bgConfigFromColor(bgColor);
    auto shipMenu = std::make_unique<UiMenu>(config, bg);

    m_ships.push_back(shipMenu.get());
    m_menu->addMenu(std::move(shipMenu));
  }
}

void HangarUiHandler::generateShipsMenus()
{
  const auto ships = m_playerView->getPlayerShips();

  auto id = 0;
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(olc::BLANK);

  for (const auto &ship : ships)
  {
    auto textConf = textConfigFromColor(ship.name, olc::BLACK);
    auto prop     = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    auto text = "Hull points: " + floatToStr(std::floor(ship.maxHullPoints), 0);
    textConf  = textConfigFromColor(text, olc::BLACK);
    prop      = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    text     = "Hull points regen: " + floatToStr(std::floor(ship.hullPointsRegen), 0);
    textConf = textConfigFromColor(text, olc::BLACK);
    prop     = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    text     = "Power points: " + floatToStr(std::floor(ship.maxPowerPoints), 0);
    textConf = textConfigFromColor(text, olc::BLACK);
    prop     = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    text     = "Power regen: " + floatToStr(std::floor(ship.powerRegen), 0);
    textConf = textConfigFromColor(text, olc::BLACK);
    prop     = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    text = "";
    for (const auto &[slot, count] : ship.slots)
    {
      if (!text.empty())
      {
        text += ", ";
      }
      text += std::to_string(count) + " " + bsgo::str(slot);
    }
    textConf = textConfigFromColor(text, olc::BLACK);
    prop     = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_ships[id]->addMenu(std::move(prop));

    ++id;
  }
}

} // namespace pge
