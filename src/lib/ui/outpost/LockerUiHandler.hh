
#pragma once

#include "IUiHandler.hh"
#include "PlayerView.hh"
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
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bsgo::ShipViewShPtr m_shipView;
  bool m_initialized{false};

  UiMenuPtr m_menu{};
  UiMenu *m_locker{};
  UiMenu *m_ship{};
  std::vector<UiMenu *> m_resources{};

  struct LockerItem
  {
    UiMenu *equip{};
  };

  std::vector<UiMenu *> m_lockerWeapons{};
  std::vector<LockerItem> m_lockerWeaponData{};

  std::vector<UiMenu *> m_lockerComputers{};
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

  void onInstallRequest(const bsgo::Item &type, const int id);
  void onUninstallRequest(const bsgo::Item &type, const int id);
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
