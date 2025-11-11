
#include "ShopUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

ShopUiHandler::ShopUiHandler(const Views &views)
  : IUiHandler("shop")
  , m_shopView(views.shopView)
  , m_playerView(views.playerView)
{
  if (nullptr == m_shopView)
  {
    throw std::invalid_argument("Expected non null shop view");
  }
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }

  subscribeToViews();
}

void ShopUiHandler::initializeMenus(const int width,
                                    const int height,
                                    sprites::TexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  constexpr auto RESOURCES_MENU_HEIGHT = 30;
  Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
            height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  Vec2i dims{viewWidth, RESOURCES_MENU_HEIGHT};

  m_resourcesMenu = generateBlankHorizontalMenu(pos, dims);

  pos.y += RESOURCES_MENU_HEIGHT;
  dims.y = viewHeight - RESOURCES_MENU_HEIGHT;
  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool ShopUiHandler::processUserInput(UserInputData &inputData)
{
  // The resources menu can't take input.
  return m_menu->processUserInput(inputData);
}

void ShopUiHandler::render(Renderer &engine) const
{
  m_resourcesMenu->render(engine);
  m_menu->render(engine);
}

namespace {
void updateBuyButtonState(UiMenu &buyButton, const bool enable)
{
  buyButton.setEnabled(enable);

  if (!enable)
  {
    buyButton.updateBgColor(colors::DARK_GREY);
  }
  else
  {
    buyButton.updateBgColor(colors::DARK_GREEN);
  }
}

void updatePricesState(const std::unordered_map<bsgo::Uuid, UiTextMenu *> &prices,
                       const std::unordered_map<bsgo::Uuid, bool> &resourcesAvailability)
{
  for (const auto &[resource, menu] : prices)
  {
    const auto available = resourcesAvailability.contains(resource)
                           && true == resourcesAvailability.at(resource);
    const auto color = available ? colors::DARK_GREEN : colors::DARK_RED;
    menu->updateTextColor(color);
  }
}
} // namespace

void ShopUiHandler::updateUi()
{
  if (!m_shopView->isReady() || !m_playerView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeShop();
  }

  for (const auto &itemData : m_itemsData)
  {
    const auto affordability = m_shopView->canPlayerAfford(itemData.itemId, itemData.itemType);
    updateBuyButtonState(*itemData.menu, affordability.canAfford);
    updatePricesState(itemData.prices, affordability.resourceAvailibility);
  }
}

void ShopUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_shopView->addListener(std::move(listener));

  listener = std::make_unique<IViewListenerProxy>(consumer);
  m_playerView->addListener(std::move(listener));
}

void ShopUiHandler::reset()
{
  m_resourcesMenu->clearChildren();
  m_items.clear();
  m_itemsData.clear();
  m_menu->clearChildren();

  m_initialized = false;
}

void ShopUiHandler::initializeShop()
{
  generateResourcesMenus();
  initializeLayout();
  generateItemsMenus();

  m_initialized = true;
}

void ShopUiHandler::generateResourcesMenus()
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

    const auto amount = std::to_string(it->amount);
    label = textConfigFromColor(amount, colorFromResourceName(it->name), TextAlignment::LEFT);
    field = std::make_unique<UiTextMenu>(config, bg, label);
    m_resourcesMenu->addMenu(std::move(field));
  }
}

void ShopUiHandler::initializeLayout()
{
  const auto palette = generatePaletteForFaction(m_playerView->getPlayerFaction());
  m_menu->updateBgColor(palette.almostOpaqueColor);

  const auto items = m_shopView->getShopItems();
  for (auto id = 0u; id < items.size(); ++id)
  {
    const MenuConfig config{.layout = MenuLayout::HORIZONTAL};
    const auto bg = bgConfigFromColor(colors::BLANK);
    auto itemMenu = std::make_unique<UiMenu>(config, bg);
    m_items.push_back(itemMenu.get());
    m_menu->addMenu(std::move(itemMenu));
  }
}

namespace {
struct PriceMenu
{
  UiMenuPtr menu{};
  std::vector<UiTextMenu *> costs{};
};

auto generatePriceMenus(const ShopItem &item) -> PriceMenu
{
  PriceMenu out{};

  out.menu = generateBlankVerticalMenu();

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto label = std::string("Cost:");
  auto text  = generateTextConfig(label, colors::GREY, 10);
  auto prop  = std::make_unique<UiTextMenu>(config, bg, text);
  out.menu->addMenu(std::move(prop));

  for (const auto &cost : item.price)
  {
    label     = cost.resource.name + ": " + std::to_string(cost.amount);
    text      = generateTextConfig(label);
    auto menu = std::make_unique<UiTextMenu>(config, bg, text);
    out.costs.push_back(menu.get());
    out.menu->addMenu(std::move(menu));
  }

  return out;
}
} // namespace

void ShopUiHandler::generateItemsMenus()
{
  const auto &gameSession = m_shopView->gameSession();
  const auto items        = m_shopView->getShopItems();

  auto itemId = 0;
  for (const auto &item : items)
  {
    ItemData data{.itemId = item.id(), .itemType = item.type()};
    m_itemsData.push_back(data);

    auto itemSection = generateItemMenus(item, gameSession);
    m_items[itemId]->addMenu(std::move(itemSection));

    auto costsSection = generatePriceMenus(item);
    auto id           = 0;
    for (const auto &costMenu : costsSection.costs)
    {
      const auto cost = item.price.at(id);

      m_itemsData.at(itemId).prices[cost.resource.dbId] = costMenu;

      ++id;
    }
    m_items[itemId]->addMenu(std::move(costsSection.menu));

    auto buySection = generateInteractiveSection("Buy",
                                                 [this, itemId]() { onPurchaseRequest(itemId); });

    m_itemsData.at(itemId).menu = buySection.button;
    updateBuyButtonState(*buySection.button, false);

    m_items[itemId]->addMenu(std::move(buySection.menu));

    ++itemId;
  }
}

auto ShopUiHandler::generateItemMenus(const ShopItem &item, const GameSession &gameSession)
  -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();
  if (item.weapon)
  {
    auto itemMenu = generateWeaponMenu(*item.weapon, gameSession.getTimeStep());
    menu->addMenu(std::move(itemMenu));
  }
  else if (item.computer)
  {
    auto itemMenu = generateComputerMenu(*item.computer, gameSession.getTimeStep());
    menu->addMenu(std::move(itemMenu));
  }

  return menu;
}

void ShopUiHandler::onPurchaseRequest(const int itemId)
{
  if (!m_playerView->isReady())
  {
    return;
  }

  const auto &purchase = m_itemsData.at(itemId);
  m_playerView->tryPurchase(purchase.itemType, purchase.itemId);
}

} // namespace pge
