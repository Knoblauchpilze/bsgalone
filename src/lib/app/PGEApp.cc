
#include "PGEApp.hh"

namespace pge {

PGEApp::PGEApp(const AppDesc &desc)
  : utils::CoreObject(desc.name)
  , olc::PixelGameEngine()
  , m_controls(controls::newState())
  , m_fixedFrame(desc.fixedFrame)
  , m_frame(desc.frame)
{
  // Initialize the application settings.
  sAppName = desc.name;
  setService("app");

  // Make sure that a coordinate frame is provided.
  if (m_frame == nullptr)
  {
    error("Unable to create PGE application", "Invalid null coordinate frame provided");
  }

  if (desc.maxFps)
  {
    m_fpsControl = FramerateControl(*desc.maxFps);
  }

  // Generate and construct the window.
  initialize(desc.dims, desc.pixRatio);
}

bool PGEApp::OnUserCreate()
{
  // The debug layer is the default layer: it is always
  // provided by the pixel game engine.
  m_dLayer = 0u;

  // Create a layer for the UI elements and enable it.
  m_uiLayer = CreateLayer();
  EnableLayer(m_uiLayer, true);

  // And finally create the two layers for the main content:
  // one allowing to draw the decal objects and one all the
  // regular ones. As the pixel game engine draws layers
  // from back to front the main content should be at the
  // back so that all other elements are displayed on top
  // of it.
  m_mLayer = CreateLayer();
  EnableLayer(m_mLayer, true);

  m_mDecalLayer = CreateLayer();
  EnableLayer(m_mDecalLayer, true);

  loadResources();

  return true;
}

bool PGEApp::OnUserUpdate(float fElapsedTime)
{
  // Handle inputs.
  InputChanges ic = handleInputs();

  // Handle user inputs.
  onInputs(m_controls, *m_frame);

  // Handle game logic.
  bool quit = onFrame(fElapsedTime);

  // Handle rendering: for each function
  // we will assign the draw target first
  // so that the function does not have
  // to handle it. We want to save the
  // current draw target to restore it
  // once the process is done.
  olc::Sprite *base = GetDrawTarget();

  RenderState res{
    .cf = *m_frame, // Coordinate frame
  };

  // Note that we usually need to clear
  // the layer at least once to `activate`
  // them: otherwise the window usually
  // stays black.
  SetDrawTarget(m_mDecalLayer);
  drawDecal(res);

  SetDrawTarget(m_mLayer);
  draw(res);

  if (hasUI())
  {
    SetDrawTarget(m_uiLayer);
    drawUI(res);
  }
  if (!hasUI() && isFirstFrame())
  {
    SetDrawTarget(m_uiLayer);
    clearLayer();
  }

  // Draw the debug layer. As it is saved in the layer `0` we need to clear it
  // when it is not displayed as it will be rendered on top of any other layer
  // and thus we would still display the last frame when it is inactive.
  // Note that we also clear it in case the debug is set to `false` from the
  // beginning of the rendering: if we don't do this nothing will be visible
  // as the `0`-th layer would never be updated.
  if (hasDebug())
  {
    SetDrawTarget(m_dLayer);
    drawDebug(res);
  }
  if (!hasDebug() && (ic.debugLayerToggled || isFirstFrame()))
  {
    SetDrawTarget(m_dLayer);
    clearLayer();
  }

  // Restore the target.
  SetDrawTarget(base);

  // Not the first frame anymore.
  m_first = false;

  if (m_fpsControl)
  {
    m_fpsControl->throttle();
  }

  return !ic.quit && !quit;
}

bool PGEApp::OnUserDestroy()
{
  cleanResources();
  return true;
}

bool PGEApp::isFirstFrame() const noexcept
{
  return m_first;
}

bool PGEApp::hasDebug() const noexcept
{
  return m_debugOn;
}

bool PGEApp::hasUI() const noexcept
{
  return m_uiOn;
}

bool PGEApp::hasCursor() const noexcept
{
  return m_cursorOn;
}

void PGEApp::setLayerTint(const Layer &layer, const olc::Pixel &tint)
{
  switch (layer)
  {
    case Layer::Draw:
      SetLayerTint(m_mLayer, tint);
      break;
    case Layer::DrawDecal:
      SetLayerTint(m_mDecalLayer, tint);
      break;
    case Layer::UI:
      SetLayerTint(m_uiLayer, tint);
      break;
    case Layer::Debug:
    default:
      SetLayerTint(m_dLayer, tint);
      break;
  }
}

void PGEApp::clearLayer()
{
  // Clear the canvas with a neutral fully transparent color.
  SetPixelMode(olc::Pixel::ALPHA);
  Clear(olc::Pixel(255, 255, 255, alpha::Transparent));
  SetPixelMode(olc::Pixel::NORMAL);
}

inline void PGEApp::initialize(const olc::vi2d &dims, const olc::vi2d &pixRatio)
{
  // Construct the window. Note that we use a pixel size
  // to screen size ratio of `1` (meaning that each pixel
  // of the viewport will be represented by a pixel on
  // the screen).
  olc::rcode c = Construct(dims.x, dims.y, pixRatio.x, pixRatio.y);

  if (c != olc::OK)
  {
    error("Could not build new frontiers application", "Initialization failed");
  }
}

auto PGEApp::handleInputs() -> PGEApp::InputChanges
{
  InputChanges ic{false, false};

  // Detect press on `Escape` key to shutdown the app.
  olc::HWButton esc = GetKey(olc::ESCAPE);
  if (esc.bReleased)
  {
    ic.quit = true;
    return ic;
  }

  if (!m_fixedFrame)
  {
    // In case we're dragging the right mouse button we
    // will update the world's position (panning). What
    // we want is to remember the position at the moment
    // of the click and then continuously move the world
    // to match the current displacement.
    if (GetMouse(1).bPressed)
    {
      m_frame->beginTranslation(GetMousePos());
    }
    if (GetMouse(1).bHeld)
    {
      m_frame->translate(GetMousePos());
    }
  }

  olc::vi2d mPos   = GetMousePos();
  m_controls.mPosX = mPos.x;
  m_controls.mPosY = mPos.y;

  if (!m_fixedFrame)
  {
    int scroll = GetMouseWheel();
    if (scroll > 0)
    {
      m_frame->zoomIn(GetMousePos());
    }
    if (scroll < 0)
    {
      m_frame->zoomOut(GetMousePos());
    }
  }

  // Handle inputs. Note that for keys apart for the
  // motion keys (or commonly used as so) we want to
  // react on the released event only.
  olc::HWButton b                        = GetKey(olc::RIGHT);
  m_controls.keys[controls::keys::RIGHT] = b.bPressed || b.bHeld;
  b                                      = GetKey(olc::D);
  m_controls.keys[controls::keys::D]     = b.bPressed || b.bHeld;

  b                                   = GetKey(olc::UP);
  m_controls.keys[controls::keys::UP] = b.bPressed || b.bHeld;
  b                                   = GetKey(olc::Z);
  m_controls.keys[controls::keys::Z]  = b.bPressed || b.bHeld;

  b                                     = GetKey(olc::LEFT);
  m_controls.keys[controls::keys::LEFT] = b.bPressed || b.bHeld;
  b                                     = GetKey(olc::Q);
  m_controls.keys[controls::keys::Q]    = b.bPressed || b.bHeld;

  b                                     = GetKey(olc::DOWN);
  m_controls.keys[controls::keys::DOWN] = b.bPressed || b.bHeld;
  b                                     = GetKey(olc::S);
  m_controls.keys[controls::keys::S]    = b.bPressed || b.bHeld;

  b                                      = GetKey(olc::SPACE);
  m_controls.keys[controls::keys::SPACE] = b.bPressed || b.bHeld;

  b                                  = GetKey(olc::P);
  m_controls.keys[controls::keys::P] = b.bReleased;

  b                                  = GetKey(olc::M);
  m_controls.keys[controls::keys::M] = b.bReleased;

  b                = GetKey(olc::TAB);
  m_controls.tab   = b.bReleased;
  b                = GetKey(olc::SHIFT);
  m_controls.shift = (b.bPressed || b.bHeld);

  auto analysis = [](const olc::HWButton &b) {
    if (b.bPressed)
    {
      return controls::ButtonState::PRESSED;
    }
    if (b.bHeld)
    {
      return controls::ButtonState::HELD;
    }
    if (b.bReleased)
    {
      return controls::ButtonState::RELEASED;
    }

    return controls::ButtonState::FREE;
  };

  m_controls.buttons[controls::mouse::LEFT]   = analysis(GetMouse(0));
  m_controls.buttons[controls::mouse::RIGHT]  = analysis(GetMouse(1));
  m_controls.buttons[controls::mouse::MIDDLE] = analysis(GetMouse(2));

  // De/activate the debug mode if needed and
  // handle general simulation control options.
  if (GetKey(olc::D).bReleased && m_controls.shift)
  {
    m_debugOn            = !m_debugOn;
    ic.debugLayerToggled = true;
  }
  if (GetKey(olc::U).bReleased)
  {
    m_uiOn = !m_uiOn;
  }

  return ic;
}

} // namespace pge
