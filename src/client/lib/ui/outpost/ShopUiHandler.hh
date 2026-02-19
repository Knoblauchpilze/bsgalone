
#pragma once

#include "GameSession.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(const Views &views);
  ~ShopUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  ShopViewShPtr m_shopView{};
  PlayerViewShPtr m_playerView{};
  bool m_initialized{false};

  UiMenuPtr m_resourcesMenu{};
  UiMenuPtr m_menu{};
  std::vector<UiMenu *> m_items{};

  struct ItemData
  {
    bsgo::Uuid itemId{};
    bsgalone::core::Item itemType{};
    UiMenu *menu{};
    std::unordered_map<bsgo::Uuid, UiTextMenu *> prices{};
  };
  std::vector<ItemData> m_itemsData{};

  void subscribeToViews();
  void reset();
  void initializeShop();
  void generateResourcesMenus();
  void initializeLayout();
  void generateItemsMenus();
  auto generateItemMenus(const ShopItem &item, const GameSession &gameSession) -> UiMenuPtr;

  void onPurchaseRequest(const int itemId);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge
