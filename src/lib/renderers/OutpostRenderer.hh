
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"

namespace pge {

class OutpostRenderer : public IRenderer
{
  public:
  OutpostRenderer(int width, int height);
  ~OutpostRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;

  private:
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge