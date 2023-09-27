
#pragma once

#include "GameUiRenderer.hh"
#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class GameRenderer : public IRenderer
{
  public:
  GameRenderer(const bsgo::Views &views, int width, int height);
  ~GameRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  GameUiRendererPtr m_uiRenderer;

  void create(const bsgo::Views &views, int width, int height);
};

} // namespace pge