
#include "App.hh"

namespace pge {

// https://opengameart.org/content/tiny-16-basic
constexpr auto DEFAULT_TEXTURE_PACK_FILE_PATH = "data/tiles/default.png";

constexpr auto DEFAULT_TEXTURE_PACK_WIDTH_IN_TILES  = 8;
constexpr auto DEFAULT_TEXTURE_PACK_HEIGHT_IN_TILES = 13;

constexpr auto DEFAULT_TEXTURE_PACK_TILE_SIZE_IN_PIXELS = 16;

const auto CURSOR_COLOR = olc::Pixel{255, 255, 0, alpha::SemiOpaque};

App::App(const AppDesc &desc)
  : PGEApp(desc)
  , m_game(nullptr)
  , m_state(nullptr)
  , m_menus()
  , m_packs(std::make_shared<sprites::TexturePack>())
{}

bool App::onFrame(float fElapsed)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return false;
  }

  if (!m_game->step(fElapsed))
  {
    info("This is game over");
  }

  return m_game->terminated();
}

void App::onInputs(const controls::State &c, const CoordinateFrame &cf)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return;
  }

  // Handle menus update and process the
  // corresponding actions.
  std::vector<ActionShPtr> actions;
  bool relevant = false;

  for (unsigned id = 0u; id < m_menus.size(); ++id)
  {
    menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
    relevant             = (relevant || ih.relevant);
  }

  if (m_state != nullptr)
  {
    menu::InputHandle ih = m_state->processUserInput(c, actions);
    relevant             = (relevant || ih.relevant);
  }

  for (unsigned id = 0u; id < actions.size(); ++id)
  {
    actions[id]->apply(*m_game);
  }

  bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
  if (lClick && !relevant)
  {
    olc::vf2d it;
    olc::vi2d tp = cf.pixelsToTilesAndIntra(olc::vi2d(c.mPosX, c.mPosY), &it);

    m_game->performAction(tp.x + it.x, tp.y + it.y);
  }

  if (c.keys[controls::keys::P])
  {
    m_game->togglePause();
  }
}

void App::loadData()
{
  // Create the game and its state.
  m_game = std::make_shared<Game>();
}

void App::loadResources()
{
  // Assign a specific tint to the regular drawing layer so that we have a built
  // in transparency.
  // We can't do it directly when drawing in the rendering function because as the
  // whole layer will be drawn as one quad in opengl with an opaque alpha, we will
  // lose this info.
  // This means that everything is indeed transparent but that's the only way for
  // now to achieve it.
  setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));

  auto pack = sprites::PackDesc{.file = DEFAULT_TEXTURE_PACK_FILE_PATH,
                                .sSize{DEFAULT_TEXTURE_PACK_TILE_SIZE_IN_PIXELS,
                                       DEFAULT_TEXTURE_PACK_TILE_SIZE_IN_PIXELS},
                                .layout{DEFAULT_TEXTURE_PACK_WIDTH_IN_TILES,
                                        DEFAULT_TEXTURE_PACK_HEIGHT_IN_TILES}};

  m_defaultPackId = m_packs->registerPack(pack);
}

void App::loadMenuResources()
{
  // Generate the game state.
  m_state = std::make_shared<GameState>(olc::vi2d(ScreenWidth(), ScreenHeight()), Screen::Home);

  m_menus = m_game->generateMenus(ScreenWidth(), ScreenHeight());
}

void App::cleanResources()
{
  if (m_packs != nullptr)
  {
    m_packs.reset();
  }
}

void App::cleanMenuResources()
{
  m_menus.clear();
}

void App::drawDecal(const RenderDesc &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::VERY_DARK_GREY);

  // In case we're not in the game screen, do nothing.
  if (m_state->getScreen() != Screen::Game)
  {
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  renderDefaultTexturePack(res.cf);
  if (hasCursor())
  {
    renderCursor(res);
  }

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::draw(const RenderDesc & /*res*/)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  // In case we're not in game mode, just render the state.
  if (m_state->getScreen() != Screen::Game)
  {
    m_state->render(this);
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawUI(const RenderDesc & /*res*/)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  // In case we're not in game mode, just render the state.
  if (m_state->getScreen() != Screen::Game)
  {
    m_state->render(this);
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  // Render the game menus.
  for (unsigned id = 0u; id < m_menus.size(); ++id)
  {
    m_menus[id]->render(this);
  }

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawDebug(const RenderDesc &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  // In case we're not in game mode, just render the state.
  if (m_state->getScreen() != Screen::Game)
  {
    m_state->render(this);
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  // Draw cursor's position.
  olc::vi2d mp = GetMousePos();
  olc::vf2d it;
  olc::vi2d mtp = res.cf.pixelsToTilesAndIntra(mp, &it);

  int h       = GetDrawTargetHeight();
  int dOffset = 15;
  DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + mp.str(), olc::CYAN);
  DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + mtp.str(), olc::CYAN);
  DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + it.str(), olc::CYAN);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::renderCursor(const RenderDesc &res)
{
  olc::vf2d it;
  const auto mouseTilePosition = res.cf.pixelsToTilesAndIntra(GetMousePos(), &it);

  SpriteDesc s;
  s.radius      = 1.0f;
  s.x           = mouseTilePosition.x;
  s.y           = mouseTilePosition.y;
  s.sprite.tint = CURSOR_COLOR;
  drawWarpedRect(s, res.cf);
}

inline void App::drawSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  olc::vf2d p = cf.tilesToPixels(t.x, t.y);

  m_packs->draw(this, t.sprite, p, t.radius * cf.tileSize());
}

inline void App::drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  auto p0 = cf.tilesToPixels(t.x, t.y + t.radius);
  auto p1 = cf.tilesToPixels(t.x, t.y);
  auto p2 = cf.tilesToPixels(t.x + t.radius, t.y);
  auto p3 = cf.tilesToPixels(t.x + t.radius, t.y + t.radius);

  auto p = std::array<olc::vf2d, 4>{p0, p1, p2, p3};
  m_packs->draw(this, t.sprite, p);
}

inline void App::drawRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  olc::vf2d p = cf.tilesToPixels(t.x, t.y);
  FillRectDecal(p, t.radius * cf.tileSize(), t.sprite.tint);
}

inline void App::drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  auto p0 = cf.tilesToPixels(t.x, t.y + t.radius);
  auto p1 = cf.tilesToPixels(t.x, t.y);
  auto p2 = cf.tilesToPixels(t.x + t.radius, t.y);
  auto p3 = cf.tilesToPixels(t.x + t.radius, t.y + t.radius);

  // See: FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
  // in the pixel game engine file.
  auto p = std::array<olc::vf2d, 4>{p0, p1, p2, p3};
  std::array<olc::vf2d, 4> uvs;
  uvs.fill({0, 0});

  std::array<olc::Pixel, 4> colors;
  colors.fill(t.sprite.tint);

  DrawExplicitDecal(nullptr, p.data(), uvs.data(), colors.data());
}

inline void App::renderDefaultTexturePack(const CoordinateFrame &cf)
{
  // Define a color gradient.
  auto tl = olc::RED;
  auto tr = olc::GREEN;

  auto bl = olc::BLUE;
  auto br = olc::YELLOW;

  for (int y = 0; y < DEFAULT_TEXTURE_PACK_HEIGHT_IN_TILES; ++y)
  {
    auto interpL = colorGradient(bl,
                                 tl,
                                 1.0f * y / DEFAULT_TEXTURE_PACK_HEIGHT_IN_TILES,
                                 alpha::Opaque);
    auto interpR = colorGradient(br,
                                 tr,
                                 1.0f * y / DEFAULT_TEXTURE_PACK_HEIGHT_IN_TILES,
                                 alpha::Opaque);

    for (int x = 0; x < DEFAULT_TEXTURE_PACK_WIDTH_IN_TILES; ++x)
    {
      SpriteDesc t;
      t.x = x;
      t.y = y;

      t.radius = 1.0f;

      t.sprite.pack   = m_defaultPackId;
      t.sprite.sprite = {x, y};
      t.sprite.tint   = colorGradient(interpL,
                                    interpR,
                                    1.0f * x / DEFAULT_TEXTURE_PACK_WIDTH_IN_TILES,
                                    alpha::Opaque);

      drawWarpedSprite(t, cf);
    }
  }
}

} // namespace pge
