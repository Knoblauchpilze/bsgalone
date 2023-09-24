
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

  // Load elements.
  loadData();
  loadMenuResources();
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

  RenderDesc res{
    *m_frame, // Coordinate frame
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

PGEApp::InputChanges PGEApp::handleInputs()
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
  m_controls.keys[controls::keys::Right] = b.bPressed || b.bHeld;

  b                                   = GetKey(olc::UP);
  m_controls.keys[controls::keys::Up] = b.bPressed || b.bHeld;

  b                                     = GetKey(olc::LEFT);
  m_controls.keys[controls::keys::Left] = b.bPressed || b.bHeld;

  b                                     = GetKey(olc::DOWN);
  m_controls.keys[controls::keys::Down] = b.bPressed || b.bHeld;

  b                                      = GetKey(olc::SPACE);
  m_controls.keys[controls::keys::Space] = b.bPressed || b.bHeld;

  b                                  = GetKey(olc::P);
  m_controls.keys[controls::keys::P] = b.bReleased;

  b = GetKey(olc::TAB), m_controls.tab = b.bReleased;

  auto analysis = [](const olc::HWButton &b) {
    if (b.bPressed)
    {
      return controls::ButtonState::Pressed;
    }
    if (b.bHeld)
    {
      return controls::ButtonState::Held;
    }
    if (b.bReleased)
    {
      return controls::ButtonState::Released;
    }

    return controls::ButtonState::Free;
  };

  m_controls.buttons[controls::mouse::Left]   = analysis(GetMouse(0));
  m_controls.buttons[controls::mouse::Right]  = analysis(GetMouse(1));
  m_controls.buttons[controls::mouse::Middle] = analysis(GetMouse(2));

  // De/activate the debug mode if needed and
  // handle general simulation control options.
  if (GetKey(olc::D).bReleased)
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
