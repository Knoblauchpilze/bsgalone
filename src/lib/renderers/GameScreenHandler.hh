
#pragma once

#include "GameScreenShipHandler.hh"
#include "GameScreenSystemHandler.hh"
#include "GameScreenUiHandler.hh"
#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class GameScreenHandler : public IScreenHandler
{
  public:
  GameScreenHandler(const bsgo::Views &views);
  ~GameScreenHandler() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c,
                        std::vector<ActionShPtr> &actions,
                        CoordinateFrame &frame) -> menu::InputHandle override;
  void updateUi() override;
  void performAction(float x, float y, const controls::State &state) override;

  private:
  GameScreenUiHandlerPtr m_uiHandler;
  GameScreenSystemHandlerPtr m_systemHandler;
  GameScreenShipHandlerPtr m_shipHandler;
};

} // namespace pge