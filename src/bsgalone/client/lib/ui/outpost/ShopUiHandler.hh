
#pragma once

#include "AbstractUiHandler.hh"
#include "GameSession.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace bsgalone::client {

class ShopUiHandler : public AbstractUiHandler
{
  public:
  ShopUiHandler(const Views &views);
  ~ShopUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ShopViewShPtr m_shopView{};
  PlayerViewShPtr m_playerView{};
  bool m_initialized{false};

  ui::UiMenuPtr m_resourcesMenu{};
  ui::UiMenuPtr m_menu{};
  std::vector<ui::UiMenu *> m_items{};

  struct ItemData
  {
    core::Uuid itemId{};
    core::Item itemType{};
    ui::UiMenu *menu{};
    std::unordered_map<core::Uuid, ui::UiTextMenu *> prices{};
  };
  std::vector<ItemData> m_itemsData{};

  void subscribeToViews();
  void reset();
  void initializeShop();
  void generateResourcesMenus();
  void initializeLayout();
  void generateItemsMenus();
  auto generateItemMenus(const ShopItem &item, const GameSession &gameSession) -> ui::UiMenuPtr;

  void onPurchaseRequest(const int itemId);
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace bsgalone::client
