
#pragma once

#include "AbilitiesUiHandler.hh"
#include "GameOverUiHandler.hh"
#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include "WeaponsUiHandler.hh"

namespace pge {

class GameScreenUiHandler : public IUiHandler
{
  public:
  GameScreenUiHandler(const bsgo::Views &views);
  ~GameScreenUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;

  enum MenuItem
  {
    NAME   = 0,
    HEALTH = 1,
    POWER  = 2,

    TARGET_NAME     = 3,
    TARGET_HEALTH   = 4,
    TARGET_POWER    = 5,
    TARGET_DISTANCE = 6,

    DOCK = 7,

    COUNT = 8,
  };
  std::vector<UiTextMenuPtr> m_menus{};

  WeaponsUiHandlerPtr m_weaponsUi{};
  AbilitiesUiHandlerPtr m_abilitiesUi{};
  GameOverUiHandlerPtr m_gameOverUi{};

  void generateShipMenus(int width, int height);
  void generateTargetMenus(int width, int height);
  void generateOutpostMenus(int width, int height);

  void updateShipUi();
  void updateTargetUi();
  void updateOutpostUi();
};

} // namespace pge
