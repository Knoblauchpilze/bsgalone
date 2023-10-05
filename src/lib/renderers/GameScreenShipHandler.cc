
#include "GameScreenShipHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

bool GameScreenShipHandler::Motion::isMoving() const
{
  return x != 0 || y != 0 || z != 0;
}

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
  , m_systemView(views.systemView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
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
  if (pass != RenderingPass::DECAL)
  {
    return;
  }

  renderShip(m_shipView->getUuid(), engine, state);
}

auto GameScreenShipHandler::processUserInput(const controls::State &c,
                                             std::vector<ActionShPtr> & /*actions*/,
                                             CoordinateFrame &frame) -> menu::InputHandle
{
  Motion motion{};
  motion.updateFromKeys(c);
  if (motion.isMoving())
  {
    moveShip(m_shipView->getUuid(), motion);
  }

  keepShipCentered(frame);
  return {};
}

void GameScreenShipHandler::updateUi() {}

void GameScreenShipHandler::performAction(float /*x*/,
                                          float /*y*/,
                                          const controls::State & /*state*/)
{}

void GameScreenShipHandler::renderShip(const bsgo::Uuid &ship,
                                       SpriteRenderer &engine,
                                       const RenderState &state) const
{
  constexpr auto SHIP_RADIUS = 0.5f;

  const auto ent = m_systemView->getEntity(ship);

  const auto pos = (*ent.transform)->position();
  SpriteDesc t;
  t.x = pos(0) - SHIP_RADIUS;
  t.y = pos(1) - SHIP_RADIUS;

  t.radius         = 2.0f * SHIP_RADIUS;
  const auto speed = (*ent.velocity)->speed();
  t.rotation       = std::atan2(speed(0), speed(1));

  t.sprite.pack   = m_class1TexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawRotatedSprite(t, state.cf);
}

void GameScreenShipHandler::moveShip(const bsgo::Uuid &ship, const Motion &motion)
{
  auto ent = m_systemView->getEntity(ship);

  Eigen::Vector3f delta = Eigen::Vector3f::Zero();
  delta(0)              = motion.x;
  delta(1)              = motion.y;
  delta(2)              = motion.z;
  delta.normalize();

  (*ent.transform)->translate(delta);
  (*ent.velocity)->accelerate(delta);
}

void GameScreenShipHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto shipUuid = m_shipView->getUuid();
  const auto ent      = m_systemView->getEntity(shipUuid);
  const auto pos      = (*ent.transform)->position();
  const olc::vf2d pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

} // namespace pge
