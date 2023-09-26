
#pragma once

#include "GameView.hh"
#include "IRenderer.hh"
#include "Menu.hh"

namespace pge {

class GameRenderer : public IRenderer
{
  public:
  GameRenderer(const bsgo::GameViewShPtr gameView, int width, int height);
  ~GameRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;

  private:
  bsgo::GameViewShPtr m_gameView;
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge