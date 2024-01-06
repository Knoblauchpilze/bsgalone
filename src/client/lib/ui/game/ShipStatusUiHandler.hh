
#pragma once

#include "IUiHandler.hh"
#include "UiBlinkingMenu.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class ShipStatusUiHandler : public IUiHandler
{
  public:
  ShipStatusUiHandler(const bsgo::Views &views);
  ~ShipStatusUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView{};

  UiBlinkingMenuPtr m_threatLabel{};

  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};

  void initializeThreatPanel(const int width, const int height);
  void initializeJumpPanel(const int width, const int height);

  void updateThreatPanel();
  void updateJumpPanel();
};

using ShipStatusUiHandlerPtr = std::unique_ptr<ShipStatusUiHandler>;

} // namespace pge
