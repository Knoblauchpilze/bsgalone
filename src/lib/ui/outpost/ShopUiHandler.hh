
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(const bsgo::Views &views, const bsgo::Services &services);
  ~ShopUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShopViewShPtr m_shopView;
  bsgo::PurchaseServiceShPtr m_purchaseService{};

  UiMenuPtr m_menu{};
  std::vector<UiMenu *> m_items{};

  struct ItemData
  {
    bsgo::Uuid itemId{};
    bsgo::Item itemType{};
    UiMenu *menu{};
    std::unordered_map<bsgo::Uuid, UiTextMenu *> prices{};
  };
  std::vector<ItemData> m_itemsData{};

  void initializeLayout();
  void generateItemsMenus();
  auto generateItemMenus(const bsgo::ShopItem &item, const int itemId) -> UiMenuPtr;
  auto generateBuySection(const int itemId) -> UiMenuPtr;

  void onPurchaseRequest(const int itemId);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge
