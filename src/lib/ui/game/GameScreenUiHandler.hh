
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include "WeaponsUiHandler.hh"

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

    TARGET_HEALTH   = 6,
    TARGET_POWER    = 7,
    TARGET_DISTANCE = 8,

    DOCK = 9,

    COUNT = 10,
  };
  std::vector<MenuShPtr> m_menus{};

  WeaponsUiHandlerPtr m_weaponsUi{};

  void generateShipMenus(int width, int height);
  void generateAbilityMenus(int width, int height);
  void generateTargetMenus(int width, int height);
  void generateOutpostMenus(int width, int height);

  void updateShipUi();
  void updateTargetUi();
  void updateOutpostUi();
};

} // namespace pge
