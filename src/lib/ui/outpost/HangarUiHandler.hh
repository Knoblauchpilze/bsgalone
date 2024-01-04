
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <core_utils/Signal.hh>
#include <memory>
#include <optional>

namespace pge {

class HangarUiHandler : public IUiHandler
{
  public:
  HangarUiHandler(const bsgo::Views &views, const bsgo::Services &services);
  ~HangarUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bsgo::ShopViewShPtr m_shopView;
  bsgo::PurchaseServiceShPtr m_purchaseService{};
  bsgo::ShipServiceShPtr m_shipService{};
  bool m_initialized{false};

  UiMenuPtr m_menu{};

  enum class State
  {
    TO_BUY,
    UNAFFORDABLE,
    TO_EQUIP,
    EQUIPED
  };

  struct ShipData
  {
    bsgo::Uuid shipDbId{};
    std::optional<bsgo::Uuid> playerShipDbId{};
    UiMenu *menu{};
    UiTextMenu *button{};
    State state{State::UNAFFORDABLE};
  };
  std::vector<ShipData> m_shipsData{};

  void initializeHangar();

  void initializeLayout();

  auto generateShipInteractiveSection(const int shipIndex) -> UiMenuPtr;
  void updateShipMenus();

  void onShipRequest(const int shipIndex);
  void onPurchaseRequest(const int shipIndex);
  void onSelectRequest(const int shipIndex);

  public:
  utils::Signal<> onShipPurchased{};
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace pge
