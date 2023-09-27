
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class GameOverRenderer : public IRenderer
{
  public:
  GameOverRenderer();
  ~GameOverRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{nullptr};
};

} // namespace pge