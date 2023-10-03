
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameScreenUiHandler : public IScreenHandler
{
  public:
  GameScreenUiHandler(const bsgo::Views &views);
  ~GameScreenUiHandler() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c,
                        std::vector<ActionShPtr> &actions,
                        CoordinateFrame &frame) -> menu::InputHandle override;
  void updateUi() override;
  void performAction(float x, float y, const controls::State &state) override;

  private:
  bsgo::ShipViewShPtr m_shipView;

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

    TARGET_HEALTH = 11,
    TARGET_POWER  = 12,

    COUNT = 13,
  };
  std::vector<MenuShPtr> m_menus{};

  void generateShipMenus(int width, int height);
  void generateAbilityMenus(int width, int height);
  void generateWeaponMenus(int width, int height);
  void generateTargetMenus(int width, int height);

  void updateShipUi();
  void updateTargetUi();
};

using GameScreenUiHandlerPtr = std::unique_ptr<GameScreenUiHandler>;

} // namespace pge