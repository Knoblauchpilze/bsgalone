
#pragma once

#include "AbilitiesUiHandler.hh"
#include "EntityUiHandler.hh"
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
  void reset() override;

  private:
  bsgo::ShipViewShPtr m_shipView;

  UiTextMenuPtr m_dock{};
  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};

  EntityUiHandlerPtr m_shipUi{};
  EntityUiHandlerPtr m_targetUi{};
  WeaponsUiHandlerPtr m_weaponsUi{};
  AbilitiesUiHandlerPtr m_abilitiesUi{};
  GameOverUiHandlerPtr m_gameOverUi{};

  void generateOutpostMenus(int width, int height);
  void generateJumpMenus(int width, int height);

  void updateOutpostUi();
  void updateJumpUi();
};

} // namespace pge
