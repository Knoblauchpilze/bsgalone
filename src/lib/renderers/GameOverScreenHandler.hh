
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class GameOverScreenHandler : public IScreenHandler
{
  public:
  GameOverScreenHandler();
  ~GameOverScreenHandler() override = default;

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
  MenuShPtr m_menu{nullptr};
};

} // namespace pge
