
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include "WeaponSlot.hh"
#include <memory>

namespace pge {

class WeaponsUiHandler : public IUiHandler
{
  public:
  WeaponsUiHandler(const bsgo::Views &views);
  virtual ~WeaponsUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bsgo::TargetViewShPtr m_targetView;

  std::vector<MenuShPtr> m_weapons{};
  std::vector<MenuShPtr> m_ranges{};
  std::vector<MenuShPtr> m_damages{};

  void generateWeaponMenus(int width, int height);
  void updateWeaponMenu(const bsgo::WeaponSlot &weapon, const int id, const bsgo::Entity &ship);
};

using WeaponsUiHandlerPtr = std::unique_ptr<WeaponsUiHandler>;

} // namespace pge
