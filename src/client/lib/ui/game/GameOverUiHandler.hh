
#pragma once

#include "AbstractMessageListener.hh"
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
  void reset() override;

  private:
  bsgo::ShipDbViewShPtr m_shipDbView{};
  bsgo::ShipViewShPtr m_shipView{};

  UiTextMenuPtr m_menu{};
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace pge
