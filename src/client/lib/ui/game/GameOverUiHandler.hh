
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class GameOverUiHandler : public IUiHandler
{
  public:
  GameOverUiHandler(const bsgo::Views &views);
  ~GameOverUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView{};
  bsgo::ShipDbViewShPtr m_shipDbView{};

  UiTextMenuPtr m_menu{};
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace pge
