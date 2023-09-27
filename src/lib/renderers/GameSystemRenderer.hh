
#pragma once

#include "IRenderer.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameSystemRenderer : public IRenderer
{
  public:
  GameSystemRenderer(const bsgo::Views &views);
  ~GameSystemRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  bsgo::SystemViewShPtr m_systemView;
};

using GameSystemRendererPtr = std::unique_ptr<GameSystemRenderer>;

} // namespace pge