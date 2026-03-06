
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace bsgalone::client {

class GameOverUiHandler : public AbstractUiHandler
{
  public:
  GameOverUiHandler(const Views &views);
  ~GameOverUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  ShipDbViewShPtr m_shipDbView{};
  ShipViewShPtr m_shipView{};

  ui::UiTextMenuPtr m_menu{};

  void subscribeToViews();
  void reset();
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace bsgalone::client
