
#include "App.hh"
#include "Screen.hh"

namespace pge {

const auto CURSOR_COLOR = olc::Pixel{255, 255, 0, alpha::SemiOpaque};

App::App(const AppDesc &desc)
  : PGEApp(desc)
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

void App::onInputs(const controls::State &c, CoordinateFrame &cf)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return;
  }

  m_game->processUserInput(c, cf);
}

void App::loadResources()
{
  m_spriteRenderer = std::make_unique<SpriteRenderer>(this);
  m_game           = std::make_shared<Game>();
  m_game->setScreen(Screen::GAME);
  m_game->togglePause();
  setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));

  m_game->generateRenderers(ScreenWidth(), ScreenHeight(), *m_spriteRenderer);
  m_game->generateInputHandlers();
  m_game->generateUiHandlers(ScreenWidth(), ScreenHeight());
}

void App::cleanResources()
{
  m_spriteRenderer.reset();
  m_game.reset();
}

void App::drawDecal(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::OFF_BLACK);

  m_game->render(*m_spriteRenderer, res, RenderingPass::DECAL);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::draw(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  m_game->render(*m_spriteRenderer, res, RenderingPass::SPRITES);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawUI(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  m_game->render(*m_spriteRenderer, res, RenderingPass::UI);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawDebug(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  m_game->render(*m_spriteRenderer, res, RenderingPass::DEBUG);

  // Draw cursor's position.
  olc::vi2d mp = GetMousePos();
  olc::vf2d it;
  olc::vi2d mtp = res.cf.pixelsToTilesAndIntra(mp, &it);

  int h       = GetDrawTargetHeight();
  int dOffset = 15;
  DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + mp.str(), olc::CYAN);
  DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + mtp.str(), olc::CYAN);
  DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + it.str(), olc::CYAN);

  DrawString(olc::vi2d(0, h / 2 + 3 * dOffset),
             "Screen            : " + str(m_game->getScreen()),
             olc::DARK_GREEN);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::renderCursor(const RenderState &res)
{
  olc::vf2d it;
  const auto mouseTilePosition = res.cf.pixelsToTilesAndIntra(GetMousePos(), &it);

  SpriteDesc s;
  s.radius      = 1.0f;
  s.x           = mouseTilePosition.x;
  s.y           = mouseTilePosition.y;
  s.sprite.tint = CURSOR_COLOR;
  m_spriteRenderer->drawWarpedRect(s, res.cf);
}

} // namespace pge
