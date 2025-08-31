
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>
#include <optional>

namespace pge {

class HangarUiHandler : public IUiHandler
{
  public:
  HangarUiHandler(const Views &views);
  ~HangarUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  PlayerViewShPtr m_playerView{};
  ShopViewShPtr m_shopView{};
  bool m_initialized{false};

  UiMenuPtr m_resourcesMenu{};
  UiMenuPtr m_menu{};

  enum class State
  {
    TO_BUY,
    UNAFFORDABLE,
    TO_EQUIP,
    EQUIPED
  };

  struct PlayerShipData
  {
    bsgo::Uuid shipDbId{};
    std::optional<bsgo::Uuid> playerShipDbId{};
    UiMenu *menu{};
    UiTextMenu *button{};
    State state{State::UNAFFORDABLE};
  };
  std::vector<PlayerShipData> m_shipsData{};

  void subscribeToViews();
  void reset();

  void initializeHangar();
  void generateResourcesMenus();
  void initializeLayout();

  void updateShipMenus();

  void onShipRequest(const int shipIndex);
  void onPurchaseRequest(const int shipIndex);
  void onSelectRequest(const int shipIndex);
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace pge
