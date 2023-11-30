
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

    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION, .propagateEventsToChildren = false};
    const auto bg = bgConfigFromColor(bgColor);
    auto itemMenu = std::make_unique<UiMenu>(config, bg);
    m_items.push_back(itemMenu.get());
    m_menu->addMenu(std::move(itemMenu));
  }
}

void ShopUiHandler::generateItemsMenus()
{
  const auto items = m_shopView->getShopItems();

  auto id = 0;
  for (const auto &item : items)
  {
    if (item.weapon)
    {
      generateWeaponMenu(id, item);
    }
    else if (item.computer)
    {
      generateComputerMenu(id, item);
    }

    generatePriceMenus(id, item);

    ++id;
  }
}

void ShopUiHandler::generateWeaponMenu(const int itemId, const bsgo::ShopItem &item)
{
  const auto &weapon = *item.weapon;

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION};
  const auto bg   = bgConfigFromColor(olc::BLANK);
  const auto text = textConfigFromColor(weapon.name, olc::WHITE);
  auto menu       = std::make_unique<UiTextMenu>(config, bg, text);
  m_items[itemId]->addMenu(std::move(menu));
}

void ShopUiHandler::generateComputerMenu(const int itemId, const bsgo::ShopItem &item)
{
  const auto &computer = *item.computer;

  const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION};
  const auto bg   = bgConfigFromColor(olc::BLANK);
  const auto text = textConfigFromColor(computer.name, olc::WHITE);
  auto menu       = std::make_unique<UiTextMenu>(config, bg, text);
  m_items[itemId]->addMenu(std::move(menu));
}

void ShopUiHandler::generatePriceMenus(const int itemId, const bsgo::ShopItem &item)
{
  for (const auto &cost : item.price)
  {
    auto text = cost.resource.name + ": " + std::to_string(cost.amount);

    const MenuConfig config{.pos = {}, .dims = DUMMY_DIMENSION};
    const auto bg       = bgConfigFromColor(olc::BLANK);
    const auto textConf = textConfigFromColor(text, olc::WHITE);
    auto menu           = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_items[itemId]->addMenu(std::move(menu));
  }
}

} // namespace pge
