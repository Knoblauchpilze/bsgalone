
#pragma once

#include "AbilitiesUiHandler.hh"
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

    TARGET_HEALTH   = 2,
    TARGET_POWER    = 3,
    TARGET_DISTANCE = 4,

    DOCK = 5,

    COUNT = 6,
  };
  std::vector<MenuShPtr> m_menus{};

  WeaponsUiHandlerPtr m_weaponsUi{};
  AbilitiesUiHandlerPtr m_abilitiesUi{};

  void generateShipMenus(int width, int height);
  void generateTargetMenus(int width, int height);
  void generateOutpostMenus(int width, int height);

  void updateShipUi();
  void updateTargetUi();
  void updateOutpostUi();
};

} // namespace pge
