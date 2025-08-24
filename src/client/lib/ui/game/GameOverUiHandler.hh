
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
  GameOverUiHandler(const Views &views);
  ~GameOverUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  ShipDbViewShPtr m_shipDbView{};
  ShipViewShPtr m_shipView{};

  UiTextMenuPtr m_menu{};

  void subscribeToViews();
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace pge
