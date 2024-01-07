
#include "PGEApp.hh"

namespace pge {

PGEApp::PGEApp(const AppDesc &desc)
  : utils::CoreObject(desc.name)
  , olc::PixelGameEngine()
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
  initialize(desc.dims, desc.enginePixToScreenPixRatio);
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
  Clear(colors::toOlcPixel(colors::Name::TRANSPARENT_WHITE));
  SetPixelMode(olc::Pixel::NORMAL);
}

inline void PGEApp::initialize(const Vec2i &dims, const Vec2i &enginePixToScreenPixRatio)
{
  const auto c = Construct(dims.x, dims.y, enginePixToScreenPixRatio.x, enginePixToScreenPixRatio.y);
  if (c != olc::OK)
  {
    error("Could not build application", "Initialization failed");
  }
}

namespace {
auto stateFromMouseButton(const olc::HWButton &button) -> controls::mouse::State
{
  if (button.bPressed)
  {
    return controls::mouse::PRESSED;
  }
  else if (button.bHeld)
  {
    return controls::mouse::HELD;
  }
  else if (button.bReleased)
  {
    return controls::mouse::RELEASED;
  }
  else
  {
    return controls::mouse::FREE;
  }
}

auto stateFromKey(const olc::HWButton &button) -> controls::keys::State
{
  if (button.bPressed)
  {
    return controls::keys::PRESSED;
  }
  else if (button.bHeld)
  {
    return controls::keys::HELD;
  }
  else if (button.bReleased)
  {
    return controls::keys::RELEASED;
  }
  else
  {
    return controls::keys::FREE;
  }
}

void updateControls(const olc::PixelGameEngine *const pge, controls::State &controls)
{
  for (auto keyId = 0; keyId < controls::keys::KEYS_COUNT; ++keyId)
  {
    const auto key = pge->GetKey(static_cast<olc::Key>(keyId));
    controls.keys[static_cast<controls::keys::Keys>(keyId)] = stateFromKey(key);
  }

  controls.tab   = controls.held(controls::keys::TAB);
  controls.shift = controls.held(controls::keys::SHIFT);

  const auto mouseLeft   = pge->GetMouse(0);
  const auto mouseRight  = pge->GetMouse(1);
  const auto mouseMiddle = pge->GetMouse(2);

  controls.buttons[controls::mouse::LEFT]   = stateFromMouseButton(mouseLeft);
  controls.buttons[controls::mouse::RIGHT]  = stateFromMouseButton(mouseRight);
  controls.buttons[controls::mouse::MIDDLE] = stateFromMouseButton(mouseMiddle);
}
} // namespace

auto PGEApp::handleInputs() -> PGEApp::InputChanges
{
  InputChanges ic{false, false};

  updateControls(this, m_controls);

  if (m_controls.released(controls::keys::ESCAPE))
  {
    ic.quit = true;
    return ic;
  }

  Vec2f mPos(GetMousePos().x, GetMousePos().y);

  if (!m_fixedFrame)
  {
    if (m_controls.pressed(controls::keys::RIGHT))
    {
      m_frame->beginTranslation(mPos);
    }
    if (m_controls.held(controls::keys::RIGHT))
    {
      m_frame->translate(mPos);
    }
  }

  m_controls.mPosX = static_cast<int>(mPos.x);
  m_controls.mPosY = static_cast<int>(mPos.y);

  if (!m_fixedFrame)
  {
    int scroll = GetMouseWheel();
    if (scroll > 0)
    {
      m_frame->zoomIn(mPos);
    }
    if (scroll < 0)
    {
      m_frame->zoomOut(mPos);
    }
  }

  if (m_controls.released(controls::keys::D) && m_controls.shift)
  {
    m_debugOn            = !m_debugOn;
    ic.debugLayerToggled = true;
  }
  if (m_controls.released(controls::keys::U) && m_controls.shift)
  {
    m_uiOn = !m_uiOn;
  }

  return ic;
}

} // namespace pge
