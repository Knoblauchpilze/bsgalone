
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "OutpostView.hh"

namespace pge {

class OutpostRenderer : public IRenderer
{
  public:
  OutpostRenderer(bsgo::OutpostViewPtr outpostView, int width, int height);
  ~OutpostRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  bsgo::OutpostViewPtr m_outpostView;
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge