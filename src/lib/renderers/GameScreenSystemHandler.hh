
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameScreenSystemHandler : public IScreenHandler
{
  public:
  GameScreenSystemHandler(const bsgo::Views &views);
  ~GameScreenSystemHandler() override = default;

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

using GameScreenSystemHandlerPtr = std::unique_ptr<GameScreenSystemHandler>;

} // namespace pge