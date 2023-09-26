
#pragma once

#include "IRenderer.hh"
#include "MapView.hh"
#include "Menu.hh"

namespace pge {

class MapRenderer : public IRenderer
{
  public:
  MapRenderer(bsgo::MapViewPtr mapView, int width, int height);
  ~MapRenderer() override = default;

  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;

  private:
  bsgo::MapViewPtr m_mapView;
  MenuShPtr m_menu{nullptr};

  void create(int width, int height);
};

} // namespace pge