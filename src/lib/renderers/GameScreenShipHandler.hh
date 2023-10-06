
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"
#include <memory>

namespace pge {
class GameScreenShipHandler : public IScreenHandler
{
  public:
  GameScreenShipHandler(const bsgo::Views &views);
  ~GameScreenShipHandler() override = default;

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
  bsgo::ShipViewShPtr m_shipView;
  bsgo::SystemViewShPtr m_systemView;

  sprites::PackId m_class1TexturesPackId{};

  struct Motion
  {
    int x{0};
    int y{0};
    int z{0};

    void updateFromKeys(const controls::State &inputs);
  };

  void renderShip(const bsgo::Uuid &ship, SpriteRenderer &engine, const RenderState &state) const;
  void renderDebug(const bsgo::Uuid &ship, SpriteRenderer &engine, const RenderState &state) const;
  void moveShip(const bsgo::Uuid &ship, const Motion &motion);
  void keepShipCentered(CoordinateFrame &frame);
};

using GameScreenShipHandlerPtr = std::unique_ptr<GameScreenShipHandler>;

} // namespace pge