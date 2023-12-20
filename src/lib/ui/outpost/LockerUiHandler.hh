
#pragma once

#include "IUiHandler.hh"
#include "LockerService.hh"
#include "PlayerView.hh"
#include "ShipView.hh"
#include "UiMenu.hh"
#include "Views.hh"
#include <core_utils/Signal.hh>
#include <memory>
#include <vector>

namespace pge {

class LockerUiHandler : public IUiHandler
{
  public:
  LockerUiHandler(const bsgo::Views &views, const bsgo::Services &services);
  ~LockerUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bsgo::ShipViewShPtr m_shipView;
  bsgo::LockerServiceShPtr m_lockerService{};
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

  std::vector<UiMenu *> m_shipWeapons{};
  std::vector<UiMenu *> m_shipComputers{};

  void initializeLocker();
  void initializeLayout();
  void initializeLockerLayout();
  void initializeShipLayout();
  void generateResourcesMenus();
  void generateLockerWeaponsMenus();
  void generateLockerComputersMenus();
  void generateShipWeaponsMenus();
  void generateShipComputersMenus();

  void onInstallRequest(const int id);
  void onUninstallRequest(const bsgo::Item &type, const int id);

  public:
  utils::Signal<> onItemEquiped{};
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
