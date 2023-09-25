
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

void App::onInputs(const controls::State &c, const CoordinateFrame &cf)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return;
  }

  std::vector<ActionShPtr> actions;
  bool relevant = false;

  const auto it = m_renderers.find(m_game->getScreen());
  if (it != m_renderers.end())
  {
    menu::InputHandle ih = it->second->processUserInput(c, actions);
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

void App::loadResources()
{
  m_spriteRenderer = std::make_unique<SpriteRenderer>(this);
  m_game           = std::make_shared<Game>();
  setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));
  m_renderers = m_game->generateRenderers(ScreenWidth(), ScreenHeight());
}

void App::cleanResources()
{
  m_spriteRenderer.reset();
  m_renderers.clear();
}

void App::drawDecal(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::VERY_DARK_GREY);

  const auto renderer = m_renderers.find(m_game->getScreen());
  if (renderer == m_renderers.end())
  {
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  renderer->second->render(*m_spriteRenderer, res);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::draw(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  const auto renderer = m_renderers.find(m_game->getScreen());
  if (renderer == m_renderers.end())
  {
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  renderer->second->render(*m_spriteRenderer, res);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawUI(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

  const auto renderer = m_renderers.find(m_game->getScreen());
  if (renderer == m_renderers.end())
  {
    SetPixelMode(olc::Pixel::NORMAL);
    return;
  }

  renderer->second->render(*m_spriteRenderer, res);

  SetPixelMode(olc::Pixel::NORMAL);
}

void App::drawDebug(const RenderState &res)
{
  // Clear rendering target.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

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
