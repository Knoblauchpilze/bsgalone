
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "TiledBackground.hh"
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
  auto processUserInput(const controls::State &c,
                        std::vector<ActionShPtr> &actions,
                        CoordinateFrame &frame) -> menu::InputHandle override;
  void updateUi() override;
  void performAction(float x, float y, const controls::State &state) override;

  private:
  bsgo::SystemViewShPtr m_systemView;
  bsgo::ShipViewShPtr m_shipView;

  sprites::PackId m_asteroidTexturesPackId{};
  TiledBackgroundPtr m_systemBackground{};

  void renderAsteroid(const Eigen::Vector3f &position,
                      SpriteRenderer &engine,
                      const RenderState &state) const;
};

using GameScreenSystemHandlerPtr = std::unique_ptr<GameScreenSystemHandler>;

} // namespace pge