
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(const bsgo::Views &views);
  ~ShopUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
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
