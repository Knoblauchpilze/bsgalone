
#include "GameScreenShipHandler.hh"
#include "EntitiesUtils.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

void GameScreenShipHandler::Motion::updateFromKeys(const controls::State &inputs)
{
  if (inputs.keys[controls::keys::LEFT] || inputs.keys[controls::keys::Q])
  {
    --x;
  }
  if (inputs.keys[controls::keys::RIGHT] || (inputs.keys[controls::keys::D] && !inputs.shift))
  {
    ++x;
  }
  if (inputs.keys[controls::keys::UP] || inputs.keys[controls::keys::Z])
  {
    ++y;
  }
  if (inputs.keys[controls::keys::DOWN] || inputs.keys[controls::keys::S])
  {
    --y;
  }
}

GameScreenShipHandler::GameScreenShipHandler(const bsgo::Views &views)
  : m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameScreenShipHandler::loadResources(int /*width*/,
                                          int /*height*/,
                                          sprites::TexturePack &texturesLoader)
{
  constexpr auto CLASS_1_TEXTURE_PACK_FILE_PATH = "data/assets/class_1.png";
  constexpr auto CLASS_1_TILE_WIDTH_PIXELS      = 460;
  constexpr auto CLASS_1_TILE_HEIGHT_PIXELS     = 783;
  auto pack = sprites::PackDesc{.file = CLASS_1_TEXTURE_PACK_FILE_PATH,
                                .sSize{CLASS_1_TILE_WIDTH_PIXELS, CLASS_1_TILE_HEIGHT_PIXELS},
                                .layout{1, 1}};

  m_class1TexturesPackId = texturesLoader.registerPack(pack);
}

void GameScreenShipHandler::render(SpriteRenderer &engine,
                                   const RenderState &state,
                                   const RenderingPass pass) const
{
  const auto bbox  = toIBoundingBox(state.cf);
  const auto ships = m_shipView->getShipsWithin(bbox);
  for (const auto &ship : ships)
  {
    if (pass == RenderingPass::DECAL)
    {
      renderShip(ship, engine, state);
    }
    if (pass == RenderingPass::DEBUG)
    {
      renderDebug(ship, engine, state);
    }
  }
}

auto GameScreenShipHandler::processUserInput(const controls::State &c,
                                             std::vector<ActionShPtr> & /*actions*/,
                                             CoordinateFrame &frame) -> menu::InputHandle
{
  Motion motion{};
  motion.updateFromKeys(c);
  auto ship = m_shipView->getPlayerShip();
  moveShip(ship, motion);

  keepShipCentered(frame);
  return {};
}

void GameScreenShipHandler::updateUi() {}

void GameScreenShipHandler::performAction(float /*x*/,
                                          float /*y*/,
                                          const controls::State & /*state*/)
{}

void GameScreenShipHandler::renderShip(const bsgo::Entity &ship,
                                       SpriteRenderer &engine,
                                       const RenderState &state) const
{
  constexpr auto SHIP_RADIUS = 0.5f;

  const auto pos = ship.access<bsgo::Transform>().position();
  SpriteDesc t;
  t.x = pos(0);
  t.y = pos(1);

  t.radius         = 2.0f * SHIP_RADIUS;
  const auto speed = ship.access<bsgo::Velocity>().speed();
  t.rotation       = std::atan2(speed(0), speed(1));

  t.sprite.pack   = m_class1TexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawRotatedSprite(t, state.cf);
}

void GameScreenShipHandler::renderDebug(const bsgo::Entity &ship,
                                        SpriteRenderer &engine,
                                        const RenderState &state) const
{
  const auto tilePos = ship.access<bsgo::Transform>().position();
  olc::vi2d pixelPos = state.cf.tilesToPixels(tilePos(0), tilePos(1));

  olc::vi2d pos    = pixelPos;
  std::string text = "accel: ";
  const auto accel = ship.access<bsgo::Velocity>().acceleration();
  text += floatToStr(accel(0));
  text += "x";
  text += floatToStr(accel(1));
  text += "x";
  text += floatToStr(accel(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_YELLOW);

  constexpr auto REASONABLE_GAP = 20;
  pos.y += REASONABLE_GAP;
  text             = "speed: ";
  const auto speed = ship.access<bsgo::Velocity>().speed();
  text += floatToStr(speed(0));
  text += "x";
  text += floatToStr(speed(1));
  text += "x";
  text += floatToStr(speed(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_GREEN);
}

void GameScreenShipHandler::moveShip(bsgo::Entity &ship, const Motion &motion)
{
  Eigen::Vector3f delta = Eigen::Vector3f::Zero();
  delta(0)              = motion.x;
  delta(1)              = motion.y;
  delta(2)              = motion.z;
  delta.normalize();

  ship.access<bsgo::Velocity>().accelerate(delta);
}

void GameScreenShipHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto ent = m_shipView->getPlayerShip();
  const auto pos = ent.access<bsgo::Transform>().position();
  const olc::vf2d pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

} // namespace pge
