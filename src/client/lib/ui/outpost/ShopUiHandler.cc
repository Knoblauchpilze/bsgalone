
#include "ShopUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

ShopUiHandler::ShopUiHandler(const bsgo::Views &views)
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
}

void ShopUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                  height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  const Vec2i dims{viewWidth, viewHeight};

  const MenuConfig config{.pos = pos, .dims = dims, .highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);
  m_menu        = std::make_unique<UiMenu>(config, bg);
}

bool ShopUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void ShopUiHandler::render(Renderer &engine) const
{
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

void ShopUiHandler::reset()
{
  m_items.clear();
  m_itemsData.clear();
  m_menu->clearChildren();

  m_initialized = false;
}

void ShopUiHandler::initializeShop()
{
  initializeLayout();
  generateItemsMenus();

  m_initialized = true;
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
auto generatePriceMenus(const bsgo::ShopItem &item) -> std::vector<UiTextMenuPtr>
{
  std::vector<UiTextMenuPtr> out;

  for (const auto &cost : item.price)
  {
    auto text = cost.resource.name + ": " + bsgo::floatToStr(cost.amount, 0);

    const MenuConfig config{.highlightable = false};
    const auto bg       = bgConfigFromColor(colors::BLANK);
    const auto textConf = textConfigFromColor(text, colors::DARK_RED);
    auto menu           = std::make_unique<UiTextMenu>(config, bg, textConf);
    out.push_back(std::move(menu));
  }

  return out;
}
} // namespace

void ShopUiHandler::generateItemsMenus()
{
  const auto items = m_shopView->getShopItems();

  auto itemId = 0;
  for (const auto &item : items)
  {
    ItemData data{.itemId = item.id(), .itemType = item.type()};
    m_itemsData.push_back(data);

    auto itemSection = generateItemMenus(item, itemId);
    m_items[itemId]->addMenu(std::move(itemSection));

    auto buySection = generateInteractiveSection("Buy",
                                                 [this, itemId]() { onPurchaseRequest(itemId); });

    m_itemsData.at(itemId).menu = buySection.button;
    updateBuyButtonState(*buySection.button, false);

    m_items[itemId]->addMenu(std::move(buySection.menu));

    ++itemId;
  }
}

auto ShopUiHandler::generateItemMenus(const bsgo::ShopItem &item, const int itemId) -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();
  if (item.weapon)
  {
    auto itemMenu = generateWeaponMenu(*item.weapon);
    menu->addMenu(std::move(itemMenu));
  }
  else if (item.computer)
  {
    auto itemMenu = generateComputerMenu(*item.computer);
    menu->addMenu(std::move(itemMenu));
  }

  auto id = 0;
  for (auto &costMenu : generatePriceMenus(item))
  {
    const auto cost = item.price.at(id);

    m_itemsData.at(itemId).prices[cost.resource.id] = costMenu.get();
    menu->addMenu(std::move(costMenu));

    ++id;
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
