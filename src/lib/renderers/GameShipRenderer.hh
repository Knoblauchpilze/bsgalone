
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameShipRenderer : public IRenderer
{
  public:
  GameShipRenderer(const bsgo::Views &views);
  ~GameShipRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;
};

using GameShipRendererPtr = std::unique_ptr<GameShipRenderer>;

} // namespace pge