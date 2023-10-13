
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"

namespace pge {

class GameScreenUiHandler : public IUiHandler
{
  public:
  GameScreenUiHandler(const bsgo::Views &views);
  virtual ~GameScreenUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bsgo::TargetViewShPtr m_targetView;

  enum MenuItem
  {
    HEALTH = 0,
    POWER  = 1,

    ABILITY_0     = 2,
    ABILITY_1     = 3,
    ABILITY_2     = 4,
    ABILITY_3     = 5,
    ABILITY_4     = 6,
    ABILITY_COUNT = 5,

    WEAPON_0     = 7,
    WEAPON_1     = 8,
    WEAPON_2     = 9,
    WEAPON_3     = 10,
    WEAPON_COUNT = 4,

    TARGET_HEALTH   = 11,
    TARGET_POWER    = 12,
    TARGET_DISTANCE = 13,

    DOCK = 14,

    COUNT = 15,
  };
  std::vector<MenuShPtr> m_menus{};

  void generateShipMenus(int width, int height);
  void generateAbilityMenus(int width, int height);
  void generateWeaponMenus(int width, int height);
  void generateTargetMenus(int width, int height);
  void generateOutpostMenus(int width, int height);

  void updateShipUi();
  void updateTargetUi();
  void updateOutpostUi();
};

} // namespace pge
