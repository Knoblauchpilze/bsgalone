
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class MapRenderer : public IRenderer
{
  public:
  MapRenderer(const bsgo::Views &views, int width, int height);
  ~MapRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge