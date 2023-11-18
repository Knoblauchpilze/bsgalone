
#pragma once

#include "IUiHandler.hh"
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
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;

  MenuShPtr m_menu{};
};

using GameOverUiHandlerPtr = std::unique_ptr<GameOverUiHandler>;

} // namespace pge
