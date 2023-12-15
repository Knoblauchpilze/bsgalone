
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"

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

  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};

  std::vector<IUiHandlerPtr> m_uis{};

  void initializeUis(const bsgo::Views &views);
  void generateJumpMenus(int width, int height);

  void updateJumpUi();
};

} // namespace pge
