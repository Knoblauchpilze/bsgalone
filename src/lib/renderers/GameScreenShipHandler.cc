
#include "GameScreenShipHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

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

auto GameScreenShipHandler::processUserInput(const controls::State & /*c*/,
                                             std::vector<ActionShPtr> & /*actions*/)
  -> menu::InputHandle
{
  return {};
}

void GameScreenShipHandler::updateUi() {}

void GameScreenShipHandler::performAction(float /*x*/,
                                          float /*y*/,
                                          const controls::State & /*state*/)
{}

void GameScreenShipHandler::renderShip(const bsgo::Uuid &uuid,
                                       SpriteRenderer &engine,
                                       const RenderState &state) const
{
  const auto shipPosition = m_systemView->getShipPosition(uuid);

  SpriteDesc t;
  t.x = shipPosition(0);
  t.y = shipPosition(1);

  t.radius = 1.0f;

  t.sprite.pack   = m_class1TexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawWarpedSprite(t, state.cf);
}

} // namespace pge
