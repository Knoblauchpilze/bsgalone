
#pragma once

#include "AbstractUiHandler.hh"
#include "PlayerView.hh"
#include "ShipView.hh"
#include "UiMenu.hh"
#include "Views.hh"
#include <memory>
#include <vector>

namespace pge {

class LockerUiHandler : public AbstractUiHandler
{
  public:
  LockerUiHandler(const Views &views);
  ~LockerUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  PlayerViewShPtr m_playerView{};
  ShipDbViewShPtr m_shipDbView{};
  bool m_initialized{false};

  ui::UiMenuPtr m_resourcesMenu{};
  ui::UiMenuPtr m_headerMenu{};
  ui::UiMenuPtr m_menu{};
  ui::UiMenu *m_locker{};
  ui::UiMenu *m_ship{};

  struct LockerItem
  {
    bsgo::Uuid itemId{};
    bsgalone::core::Item itemType{};
    ui::UiMenu *button{};
  };

  std::vector<ui::UiMenu *> m_lockerWeapons{};
  std::vector<ui::UiMenu *> m_lockerComputers{};
  std::vector<LockerItem> m_lockerItemsData{};

  struct ShipItem
  {
    bsgo::Uuid itemId{};
    bsgalone::core::Item itemType{};
  };

  std::vector<ui::UiMenu *> m_shipWeapons{};
  std::vector<ui::UiMenu *> m_shipComputers{};
  std::vector<ShipItem> m_shipItemsData{};

  void subscribeToViews();
  void reset();
  void initializeLocker();
  void initializeLayout();
  void initializeLockerLayout();
  void initializeShipLayout();
  void generateResourcesMenus();
  void generateLockerWeaponsMenus();
  void generateLockerComputersMenus();
  void generateShipWeaponsMenus();
  void generateShipComputersMenus();

  void onInstallRequest(const int itemId);
  void onUninstallRequest(const int itemId);
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
