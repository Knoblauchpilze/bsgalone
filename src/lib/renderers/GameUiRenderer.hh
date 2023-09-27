
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameUiRenderer : public IRenderer
{
  public:
  GameUiRenderer(const bsgo::Views &views, int width, int height);
  ~GameUiRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

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

    COUNT = 11,
  };
  std::vector<MenuShPtr> m_menus{};

  void create(int width, int height);
  void generateAbilityMenus(int width, int height);
  void generateWeaponMenus(int width, int height);
};

using GameUiRendererPtr = std::unique_ptr<GameUiRenderer>;

} // namespace pge