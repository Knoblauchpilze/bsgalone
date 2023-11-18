
#pragma once

#include "IUiHandler.hh"
#include "PlayerView.hh"
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

  private:
  bsgo::PlayerViewShPtr m_playerView;
  bsgo::ShipViewShPtr m_shipView;

  MenuShPtr m_menu{};
  MenuShPtr m_locker{};
  MenuShPtr m_ship{};
  std::vector<MenuShPtr> m_resources{};
  std::vector<MenuShPtr> m_lockerWeapons{};
  std::vector<MenuShPtr> m_lockerComputers{};
  std::vector<MenuShPtr> m_shipWeapons{};
  std::vector<MenuShPtr> m_shipComputers{};

  void initializeLayout();
  void initializeLockerLayout();
  void initializeShipLayout();
  void generateResourcesMenus();
  void generateLockerWeaponsMenus();
  void generateLockerComputersMenus();
  void generateShipWeaponsMenus();
  void generateShipComputersMenus();
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
