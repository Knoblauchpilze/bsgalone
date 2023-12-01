
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
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

  UiMenuPtr m_menu{};
  std::vector<UiMenu *> m_items{};

  struct BuyButton
  {
    bsgo::Uuid itemId{};
    bsgo::Item itemType{};
    UiMenu *menu{};
  };
  std::vector<BuyButton> m_buyButtons{};

  void initializeLayout();
  void generateItemsMenus();
  auto generateItemMenus(const bsgo::ShopItem &item) -> UiMenuPtr;

  void onPurchaseRequest(const bsgo::Uuid &itemId, const bsgo::Item &itemType);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge
