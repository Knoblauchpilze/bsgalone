
#pragma once

#include "GameSystemRenderer.hh"
#include "GameUiRenderer.hh"
#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class GameRenderer : public IRenderer
{
  public:
  GameRenderer(const bsgo::Views &views);
  ~GameRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  GameUiRendererPtr m_uiRenderer;
  GameSystemRendererPtr m_systemRenderer;
};

} // namespace pge