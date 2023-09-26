
#pragma once

#include "GameView.hh"
#include "IRenderer.hh"
#include "Menu.hh"

namespace pge {

class GameOverRenderer : public IRenderer
{
  public:
  GameOverRenderer(const bsgo::GameViewShPtr gameView, int width, int height);
  ~GameOverRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  bsgo::GameViewShPtr m_gameView;
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge