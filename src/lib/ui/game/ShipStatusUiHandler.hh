
#pragma once

#include "IUiHandler.hh"
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
  bsgo::ServerViewShPtr m_serverView{};

  UiMenuPtr m_jumpPanel{};
  UiTextMenu *m_jumpDestination{};
  UiTextMenu *m_jumpTime{};
};

using ShipStatusUiHandlerPtr = std::unique_ptr<ShipStatusUiHandler>;

} // namespace pge
