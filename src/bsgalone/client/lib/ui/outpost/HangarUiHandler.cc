
#include "HangarUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace bsgalone::client {

HangarUiHandler::HangarUiHandler(const Views &views)
  : AbstractUiHandler("hangar")
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }

  subscribeToViews();
}

void HangarUiHandler::initializeMenus(const int width,
                                      const int height,
                                      pge::sprites::TexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  constexpr auto RESOURCES_MENU_HEIGHT = 30;
  pge::Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                 height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  pge::Vec2i dims{viewWidth, RESOURCES_MENU_HEIGHT};

  m_resourcesMenu = generateBlankHorizontalMenu(pos, dims);

  pos.y += RESOURCES_MENU_HEIGHT;
  dims.y = viewHeight - RESOURCES_MENU_HEIGHT;
  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool HangarUiHandler::processUserInput(ui::UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void HangarUiHandler::render(pge::Renderer &engine) const
{
  m_resourcesMenu->render(engine);
  m_menu->render(engine);
}

void HangarUiHandler::updateUi()
{
  if (!m_gameView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeHangar();
  }
}

void HangarUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_gameView->addListener(std::move(listener));
}

void HangarUiHandler::reset()
{
  m_resourcesMenu->clearChildren();
  m_shipsData.clear();
  m_menu->clearChildren();

  m_initialized = false;
}

void HangarUiHandler::initializeHangar()
{
  generateResourcesMenus();
  initializeLayout();
  updateShipMenus();
  m_initialized = true;
}

void HangarUiHandler::generateResourcesMenus()
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

namespace {
auto generateShipDescription(const core::ShipData &ship) -> ui::UiMenuPtr
{
  const ui::MenuConfig config{.highlightable = false};
  auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  auto desc = std::make_unique<ui::UiMenu>(config, bg);

  desc->addMenu(generateSpacer());

  bg         = ui::bgConfigFromColor(pge::colors::BLANK);
  auto label = core::capitalizeString(ship.name + " (" + core::str(ship.shipClass) + ")");
  auto text  = generateTextConfig(label, pge::colors::GREY, 10);
  auto prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = core::floatToStr(ship.maxHullPoints, 0) + " hull points (+"
          + core::floatToStr(ship.hullPointsRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = core::floatToStr(ship.maxPowerPoints, 0) + " power (+"
          + core::floatToStr(ship.powerRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = core::floatToStr(ship.speed, 2) + "m/s (+" + core::floatToStr(ship.acceleration, 2)
          + "m/s2)";
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = "";
  for (const auto &[slot, count] : ship.slots)
  {
    if (!label.empty())
    {
      label += ", ";
    }
    label += std::to_string(count) + " " + str(slot);
    if (count > 1)
    {
      label += "s";
    }
  }
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  desc->addMenu(generateSpacer());

  return desc;
}

} // namespace

void HangarUiHandler::initializeLayout()
{
  const auto faction  = m_gameView->getPlayerFaction();
  const auto palette  = generatePaletteForFaction(faction);
  const auto allShips = m_gameView->getAllShips();

  const ui::MenuConfig config{.layout = ui::MenuLayout::HORIZONTAL, .highlightable = false};
  const auto bg = ui::bgConfigFromColor(palette.almostOpaqueColor);

  auto shipIndex = 0;
  for (const auto &ship : allShips)
  {
    auto shipMenu = std::make_unique<ui::UiMenu>(config, bg);

    PlayerShipData data{.shipDbId = ship.dbId, .menu = shipMenu.get()};
    m_shipsData.emplace_back(std::move(data));

    auto shipDesc = generateShipDescription(ship);
    shipMenu->addMenu(std::move(shipDesc));

    auto section = generateInteractiveSection("", [this, shipIndex]() { onShipRequest(shipIndex); });
    m_shipsData.at(shipIndex).button = section.button;

    shipMenu->addMenu(std::move(section.menu));

    m_menu->addMenu(std::move(shipMenu));

    ++shipIndex;
  }
}

namespace {
constexpr auto BUY_SHIP_BUTTON_TEXT = "Purchase";
constexpr auto USE_SHIP_BUTTON_TEXT = "Select";

auto getPlayerShipWithId(const std::vector<core::PlayerShipData> &ships, const core::Uuid shipId)
  -> std::optional<core::PlayerShipData>
{
  for (const auto &ship : ships)
  {
    if (shipId == ship.shipId)
    {
      return ship;
    }
  }

  return {};
}
} // namespace

void HangarUiHandler::updateShipMenus()
{
  const auto ships = m_gameView->getPlayerShips();

  auto shipIndex = 0;
  for (auto &shipData : m_shipsData)
  {
    const auto maybePlayerShip = getPlayerShipWithId(ships, shipData.shipDbId);

    if (!maybePlayerShip)
    {
      shipData.playerShipDbId.reset();
      shipData.button->setText(BUY_SHIP_BUTTON_TEXT);
      const auto affordability = m_gameView->canPlayerAfford(shipData.shipDbId, core::Item::SHIP);

      shipData.button->setEnabled(affordability.canAfford);
      shipData.button->setHighlightable(affordability.canAfford);
      if (affordability.canAfford)
      {
        shipData.state = State::TO_BUY;
        shipData.button->updateBgColor(pge::colors::DARK_GREEN);
      }
      else
      {
        shipData.state = State::UNAFFORDABLE;
        shipData.button->updateBgColor(pge::colors::DARK_RED);
      }
    }
    else
    {
      shipData.playerShipDbId = maybePlayerShip->dbId;
      shipData.button->setText(USE_SHIP_BUTTON_TEXT);

      shipData.button->setEnabled(!maybePlayerShip->active);
      if (maybePlayerShip->active)
      {
        shipData.state = State::EQUIPED;
        shipData.button->updateBgColor(pge::colors::DARK_GREY);
      }
      else
      {
        shipData.state = State::TO_EQUIP;
        shipData.button->updateBgColor(pge::colors::DARK_GREEN);
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
  if (!m_gameView->isReady())
  {
    return;
  }

  const auto &data = m_shipsData.at(shipIndex);
  m_gameView->tryPurchase(core::Item::SHIP, data.shipDbId);
}

void HangarUiHandler::onSelectRequest(const int shipIndex)
{
  const auto &data = m_shipsData.at(shipIndex);
  m_gameView->trySelectShip(*data.playerShipDbId);
}

} // namespace bsgalone::client
