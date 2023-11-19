
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include "WeaponSlotComponent.hh"
#include <memory>

namespace pge {

class WeaponsUiHandler : public IUiHandler
{
  public:
  WeaponsUiHandler(const bsgo::Views &views);
  ~WeaponsUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bool m_initialized{false};

  std::vector<MenuShPtr> m_weapons{};
  std::vector<MenuShPtr> m_ranges{};
  std::vector<MenuShPtr> m_damages{};
  std::vector<MenuShPtr> m_statuses{};

  void generateWeaponsMenus(int width, int height);
  void initializeWeapons();
  void updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id);
};

using WeaponsUiHandlerPtr = std::unique_ptr<WeaponsUiHandler>;

} // namespace pge
