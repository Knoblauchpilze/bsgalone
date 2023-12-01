
#include "ShopUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

namespace pge {

ShopUiHandler::ShopUiHandler(const bsgo::Views &views)
  : IUiHandler("shop")
  , m_shopView(views.shopView)
{
  if (nullptr == m_shopView)
  {
    throw std::invalid_argument("Expected non null shop view");
  }
}

void ShopUiHandler::initializeMenus(const int width, const int height)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  const olc::vi2d pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                      height - viewHeight - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS};
  const olc::vi2d dims{viewWidth, viewHeight};

  const MenuConfig config{.pos = pos, .dims = dims, .highlightable = false};
  const auto bg = bgConfigFromColor(olc::DARK_MAGENTA);
  m_menu        = std::make_unique<UiMenu>(config, bg);

  initializeLayout();
  generateItemsMenus();
}

bool ShopUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void ShopUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void ShopUiHandler::updateUi() {}

constexpr auto DUMMY_PIXEL_DIMENSION = 10;
const olc::vi2d DUMMY_DIMENSION{DUMMY_PIXEL_DIMENSION, DUMMY_PIXEL_DIMENSION};

void ShopUiHandler::initializeLayout()
{
  const auto items = m_shopView->getShopItems();
  for (auto id = 0u; id < items.size(); ++id)
  {
    auto bgColor = (items[id].computer ? olc::VERY_DARK_YELLOW : olc::VERY_DARK_RED);

    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .layout = MenuLayout::HORIZONTAL};
    const auto bg = bgConfigFromColor(bgColor);
    auto itemMenu = std::make_unique<UiMenu>(config, bg);
    m_items.push_back(itemMenu.get());
    m_menu->addMenu(std::move(itemMenu));
  }
}

namespace {
auto generateWeaponMenu(const bsgo::Weapon &weapon) -> UiTextMenuPtr
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .highlightable = false};
  const auto bg   = bgConfigFromColor(olc::BLANK);
  const auto text = textConfigFromColor(weapon.name, olc::WHITE);
  return std::make_unique<UiTextMenu>(config, bg, text);
}

auto generateComputerMenu(const bsgo::Computer &computer) -> UiTextMenuPtr
{
  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .highlightable = false};
  const auto bg   = bgConfigFromColor(olc::BLANK);
  const auto text = textConfigFromColor(computer.name, olc::WHITE);
  return std::make_unique<UiTextMenu>(config, bg, text);
}

auto generatePriceMenus(const bsgo::ShopItem &item) -> std::vector<UiTextMenuPtr>
{
  std::vector<UiTextMenuPtr> out;

  for (const auto &cost : item.price)
  {
    auto text = cost.resource.name + ": " + std::to_string(cost.amount);

    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .highlightable = false};
    const auto bg       = bgConfigFromColor(olc::BLANK);
    const auto textConf = textConfigFromColor(text, olc::WHITE);
    auto menu           = std::make_unique<UiTextMenu>(config, bg, textConf);
    out.push_back(std::move(menu));
  }

  return out;
}

auto generateBuySection(const std::optional<ClickCallback> &clickCallback) -> UiMenuPtr
{
  auto middleSection = generateBlankVerticalMenu();
  middleSection->addMenu(generateSpacer());

  MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION};
  olc::Pixel bgColor;
  if (clickCallback)
  {
    config.clickCallback = *clickCallback;
    bgColor              = olc::DARK_GREEN;
  }
  else
  {
    config.highlightable = false;
    bgColor              = olc::VERY_DARK_GREY;
  }

  const auto bg       = bgConfigFromColor(bgColor);
  const auto textConf = textConfigFromColor("Buy", olc::WHITE);
  auto buyButton      = std::make_unique<UiTextMenu>(config, bg, textConf);
  middleSection->addMenu(std::move(buyButton));

  middleSection->addMenu(generateSpacer());

  auto menu = generateBlankHorizontalMenu();
  menu->addMenu(generateSpacer());
  menu->addMenu(std::move(middleSection));
  menu->addMenu(generateSpacer());

  return menu;
}

} // namespace

void ShopUiHandler::generateItemsMenus()
{
  const auto items = m_shopView->getShopItems();

  auto itemId = 0;
  for (const auto &item : items)
  {
    auto itemSection = generateItemMenus(item);
    m_items[itemId]->addMenu(std::move(itemSection));

    auto dbId       = item.id();
    auto itemType   = item.type();
    auto buySection = generateBuySection(
      [this, dbId, itemType]() { onPurchaseRequest(dbId, itemType); });
    m_items[itemId]->addMenu(std::move(buySection));

    ++itemId;
  }
}

auto ShopUiHandler::generateItemMenus(const bsgo::ShopItem &item) -> UiMenuPtr
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

  for (auto &cost : generatePriceMenus(item))
  {
    menu->addMenu(std::move(cost));
  }

  return menu;
}

void ShopUiHandler::onPurchaseRequest(const bsgo::Uuid &itemId, const bsgo::Item &itemType)
{
  /// TODO: Handle the purchase request.
  warn("should buy " + bsgo::str(itemId) + " with type " + bsgo::str(itemType));
}

} // namespace pge
