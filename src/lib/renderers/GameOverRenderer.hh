
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"

namespace pge {

class GameOverRenderer : public IRenderer
{
  public:
  GameOverRenderer(int width, int height);
  ~GameOverRenderer() override = default;

  void render(olc::PixelGameEngine *pge, const RenderState &state) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;

  private:
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge