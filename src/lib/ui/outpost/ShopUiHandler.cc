
#include "ShopUiHandler.hh"
#include "Constants.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

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
  m_menu = generateMenu(pos, dims, "", "shop", olc::DARK_MAGENTA, {olc::WHITE});

  initializeLayout();
  generateItemsMenus();
}

auto ShopUiHandler::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_menu->processUserInput(c, actions);
}

void ShopUiHandler::render(SpriteRenderer &engine) const
{
  m_menu->render(engine.getRenderer());
}

void ShopUiHandler::updateUi() {}

void ShopUiHandler::initializeLayout()
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};

  const auto items = m_shopView->getShopItems();
  for (auto id = 0u; id < items.size(); ++id)
  {
    auto text = "base_item" + std::to_string(id);

    auto bgColor = (items[id].computer ? olc::VERY_DARK_YELLOW : olc::VERY_DARK_RED);

    auto itemMenu = generateSlotMenu(pos, dims, "", text, bgColor, {olc::WHITE}, true);
    m_menu->addMenu(itemMenu);
    m_items.push_back(itemMenu);
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
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  const auto &weapon = *item.weapon;

  auto name = generateMenu(pos, dims, weapon.name, "name", transparentBg);
  m_items[itemId]->addMenu(name);
}

void ShopUiHandler::generateComputerMenu(const int itemId, const bsgo::ShopItem &item)
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  const auto &computer = *item.computer;

  auto name = generateMenu(pos, dims, computer.name, "name", transparentBg);
  m_items[itemId]->addMenu(name);
}

void ShopUiHandler::generatePriceMenus(const int itemId, const bsgo::ShopItem &item)
{
  const olc::vi2d pos{};
  const olc::vi2d dims{10, 10};
  olc::Pixel transparentBg = olc::Pixel{0, 0, 0, alpha::Transparent};

  for (const auto &cost : item.price)
  {
    auto text     = cost.resource.name + ": " + std::to_string(cost.amount);
    auto costMenu = generateMenu(pos, dims, text, "name", transparentBg);
    m_items[itemId]->addMenu(costMenu);
  }
}

} // namespace pge
