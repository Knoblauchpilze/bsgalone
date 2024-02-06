
#pragma once

#include "IUiHandler.hh"
#include "LockerService.hh"
#include "PlayerView.hh"
#include "ShipView.hh"
#include "UiMenu.hh"
#include "Views.hh"
#include <memory>
#include <vector>

namespace pge {

class LockerUiHandler : public IUiHandler
{
  public:
  LockerUiHandler(const bsgo::Views &views);
  ~LockerUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::PlayerViewShPtr m_playerView{};
  bsgo::ShipDbViewShPtr m_shipDbView{};
  bool m_initialized{false};

  UiMenuPtr m_menu{};
  UiMenu *m_locker{};
  UiMenu *m_ship{};
  std::vector<UiMenu *> m_resources{};

  struct LockerItem
  {
    bsgo::Uuid itemId{};
    bsgo::Item itemType{};
    UiMenu *button{};
  };

  std::vector<UiMenu *> m_lockerWeapons{};
  std::vector<UiMenu *> m_lockerComputers{};
  std::vector<LockerItem> m_lockerItemsData{};

  struct ShipItem
  {
    bsgo::Uuid itemId{};
    bsgo::Item itemType{};
  };

  std::vector<UiMenu *> m_shipWeapons{};
  std::vector<UiMenu *> m_shipComputers{};
  std::vector<ShipItem> m_shipItemsData{};

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
