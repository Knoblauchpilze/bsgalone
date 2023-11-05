
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(const bsgo::Views &views);
  virtual ~ShopUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShopViewShPtr m_shopView;

  MenuShPtr m_menu{};
  std::vector<MenuShPtr> m_items{};

  void initializeLayout();
  void generateItemsMenus();
  void generateWeaponMenu(const int itemId, const bsgo::ShopItem &item);
  void generateComputerMenu(const int itemId, const bsgo::ShopItem &item);
  void generatePriceMenus(const int itemId, const bsgo::ShopItem &item);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge