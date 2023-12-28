
#include "HangarUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

HangarUiHandler::HangarUiHandler(const bsgo::Views &views, const bsgo::Services &services)
  : IUiHandler("hangar")
  , m_playerView(views.playerView)
  , m_shopView(views.shopView)
  , m_purchaseService(services.purchase)
  , m_shipService(services.ship)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
  if (nullptr == m_shopView)
  {
    throw std::invalid_argument("Expected non null shop view");
  }
  if (nullptr == m_purchaseService)
  {
    throw std::invalid_argument("Expected non null purchase service");
  }
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
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
}

void HangarUiHandler::reset()
{
  m_shipsData.clear();
  m_menu->clearChildren();
  m_initialized = false;
}

void HangarUiHandler::initializeHangar()
{
  initializeLayout();
  updateShipMenus();
  m_initialized = true;
}

namespace {
constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};

auto generateShipDescription(const bsgo::Ship &ship) -> UiMenuPtr
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .highlightable = false};
  auto bg = bgConfigFromColor(olc::BLANK);

  auto desc = std::make_unique<UiMenu>(config, bg);

  desc->addMenu(generateSpacer());

  bg         = bgConfigFromColor(olc::BLANK);
  auto label = ship.name + " (" + bsgo::str(ship.shipClass) + ")";
  auto text  = textConfigFromColor(ship.name, olc::WHITE);
  auto prop  = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = floatToStr(ship.maxHullPoints, 0) + " hull points (+"
          + floatToStr(ship.hullPointsRegen, 2) + "/s)";
  text = textConfigFromColor(label, olc::WHITE);
  prop = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = floatToStr(ship.maxPowerPoints, 0) + " power (+" + floatToStr(ship.powerRegen, 2) + "/s)";
  text  = textConfigFromColor(label, olc::WHITE);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = floatToStr(ship.speed, 2) + "m/s";
  text  = textConfigFromColor(label, olc::WHITE);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = "";
  for (const auto &[slot, count] : ship.slots)
  {
    if (!label.empty())
    {
      label += ", ";
    }
    label += std::to_string(count) + " " + bsgo::str(slot);
    if (count > 1)
    {
      label += "s";
    }
  }
  text = textConfigFromColor(label, olc::WHITE);
  prop = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  desc->addMenu(generateSpacer());

  return desc;
}

} // namespace

void HangarUiHandler::initializeLayout()
{
  const auto faction  = m_playerView->getPlayerFaction();
  const auto allShips = m_shopView->getAllShipsForFaction(faction);

  const MenuConfig config{.pos           = {},
                          .dims          = DUMMY_DIMENSION,
                          .layout        = MenuLayout::HORIZONTAL,
                          .highlightable = false};
  const auto bg = bgConfigFromColor(colorFromFaction(faction));

  auto shipIndex = 0;
  for (const auto &ship : allShips)
  {
    auto shipMenu = std::make_unique<UiMenu>(config, bg);

    ShipData data{.shipDbId = ship.id, .menu = shipMenu.get()};
    m_shipsData.emplace_back(std::move(data));

    auto shipDesc = generateShipDescription(ship);
    shipMenu->addMenu(std::move(shipDesc));

    auto section = generateShipInteractiveSection(shipIndex);
    shipMenu->addMenu(std::move(section));

    m_menu->addMenu(std::move(shipMenu));

    ++shipIndex;
  }
}

auto HangarUiHandler::generateShipInteractiveSection(const int shipIndex) -> UiMenuPtr
{
  auto middleSection = generateBlankVerticalMenu();
  middleSection->addMenu(generateSpacer());
  middleSection->addMenu(generateSpacer());

  MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .clickCallback = [this, shipIndex]() {
                      onShipRequest(shipIndex);
                    }};

  const auto bg       = bgConfigFromColor(olc::DARK_GREY);
  const auto textConf = textConfigFromColor("", olc::WHITE);
  auto button         = std::make_unique<UiTextMenu>(config, bg, textConf);

  m_shipsData.at(shipIndex).button = button.get();

  middleSection->addMenu(std::move(button));

  middleSection->addMenu(generateSpacer());
  middleSection->addMenu(generateSpacer());

  auto menu = generateBlankHorizontalMenu();
  menu->addMenu(generateSpacer());
  menu->addMenu(std::move(middleSection));
  menu->addMenu(generateSpacer());

  return menu;
}

namespace {
constexpr auto BUY_SHIP_BUTTON_TEXT = "Purchase";
constexpr auto USE_SHIP_BUTTON_TEXT = "Select";

auto getPlayerShipWithId(const std::vector<bsgo::PlayerShip> &ships, const bsgo::Uuid &shipId)
  -> std::optional<bsgo::PlayerShip>
{
  for (const auto &ship : ships)
  {
    if (shipId == ship.ship)
    {
      return ship;
    }
  }

  return {};
}
} // namespace

void HangarUiHandler::updateShipMenus()
{
  const auto ships = m_playerView->getPlayerShips();

  auto shipIndex = 0;
  for (auto &shipData : m_shipsData)
  {
    const auto maybePlayerShip = getPlayerShipWithId(ships, shipData.shipDbId);

    if (!maybePlayerShip)
    {
      shipData.button->setText(BUY_SHIP_BUTTON_TEXT);
      const auto affordability = m_shopView->canPlayerAfford(shipData.shipDbId, bsgo::Item::SHIP);

      shipData.button->setEnabled(affordability.canAfford);
      shipData.button->setHighlightable(affordability.canAfford);
      if (affordability.canAfford)
      {
        shipData.state = State::TO_BUY;
        shipData.button->updateBgColor(olc::DARK_GREEN);
      }
      else
      {
        shipData.state = State::UNAFFORDABLE;
        shipData.button->updateBgColor(olc::DARK_RED);
      }
    }
    else
    {
      shipData.button->setText(USE_SHIP_BUTTON_TEXT);

      shipData.button->setEnabled(!maybePlayerShip->active);
      if (maybePlayerShip->active)
      {
        shipData.state = State::EQUIPED;
        shipData.button->updateBgColor(olc::DARK_GREY);
      }
      else
      {
        shipData.state = State::TO_EQUIP;
        shipData.button->updateBgColor(olc::DARK_GREEN);
      }
    }

    ++shipIndex;
  }
}

void HangarUiHandler::onShipRequest(const int shipIndex)
{
  const auto data = m_shipsData.at(shipIndex);
  switch (data.state)
  {
    case State::TO_BUY:
      onPurchaseRequest(shipIndex);
      break;
    case State::TO_EQUIP:
      onSelectRequest(shipIndex);
      break;
    default:
      error("Failed to handle ship request",
            "Invalid state " + std::to_string(static_cast<int>(data.state)));
      break;
  }
}

void HangarUiHandler::onPurchaseRequest(const int shipIndex)
{
  warn("should handle purchase request for " + std::to_string(shipIndex));
  onShipPurchased.safeEmit("onPurchaseRequest");
}

void HangarUiHandler::onSelectRequest(const int shipIndex)
{
  warn("should handle equip request for " + std::to_string(shipIndex));
  onShipSelected.safeEmit("onSelectRequest");
}

} // namespace pge
