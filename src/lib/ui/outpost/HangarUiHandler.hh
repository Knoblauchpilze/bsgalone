
#pragma once

#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class HangarUiHandler : public IUiHandler
{
  public:
  HangarUiHandler(const bsgo::Views &views);
  ~HangarUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bsgo::ShopViewShPtr m_shopView;
  bool m_initialized{false};

  UiMenuPtr m_menu{};

  struct ShipData
  {
    bsgo::Uuid shipDbId{};
    UiMenu *menu{};
    UiTextMenu *button{};
  };
  std::vector<ShipData> m_shipsData{};

  void initializeHangar();

  void initializeLayout();

  auto generateShipInteractiveSection(const int itemId) -> UiMenuPtr;
  void updateShipMenus();
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace pge
