
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
  ShopUiHandler(const bsgo::Views &views);
  ~ShopUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::ShopViewShPtr m_shopView{};
  bsgo::PlayerViewShPtr m_playerView{};
  bool m_initialized{false};

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

  void initializeShop();
  void initializeLayout();
  void generateItemsMenus();
  auto generateItemMenus(const bsgo::ShopItem &item) -> UiMenuPtr;

  void onPurchaseRequest(const int itemId);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge
