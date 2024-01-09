
#include "PGEApp.hh"
#include "ColorConversion.hh"
#include "PgeWrapper.hh"

namespace pge {

PGEApp::PGEApp(const AppDesc &desc)
  : utils::CoreObject(desc.name)
  , m_fixedFrame(desc.fixedFrame)
  , m_frame(desc.frame)
{
  setService("app");

  if (m_frame == nullptr)
  {
    error("Unable to create PGE application", "Invalid null coordinate frame provided");
  }

  if (desc.maxFps)
  {
    m_fpsControl = FramerateControl(*desc.maxFps);
  }

  // Generate and construct the window.
  initialize(desc.name, desc.dims, desc.enginePixToScreenPixRatio);
}

PGEApp::~PGEApp()
{
  // Intentionally empty.
}

void PGEApp::run()
{
  m_engine->Start();
}

bool PGEApp::isFirstFrame() const noexcept
{
  return m_first;
}

bool PGEApp::hasDebug() const noexcept
{
  return m_debugOn;
}

bool PGEApp::hasUi() const noexcept
{
  return m_uiOn;
}

void PGEApp::setLayerTint(const Layer &layer, const Color &tint)
{
  const auto olcTint = colors::toOlcPixel(tint);

  switch (layer)
  {
    case Layer::DRAW:
      m_engine->SetLayerTint(m_drawLayer, olcTint);
      break;
    case Layer::DECAL:
      m_engine->SetLayerTint(m_decalLayer, olcTint);
      break;
    case Layer::UI:
      m_engine->SetLayerTint(m_uiLayer, olcTint);
      break;
    case Layer::DEBUG:
    default:
      m_engine->SetLayerTint(m_debugLayer, olcTint);
      break;
  }
}

inline void PGEApp::initialize(const std::string &appName,
                               const Vec2i &dims,
                               const Vec2i &enginePixToScreenPixRatio)
{
  const WrapperCallbacks callbacks{.createCallback = [this]() { return onCreate(); },
                                   .updateCallback =
                                     [this](const float elapsedTime) {
                                       return onUpdate(elapsedTime);
                                     },
                                   .destroyCallback = [this]() { return onDestroy(); }};
  m_engine           = std::make_unique<PgeWrapper>(callbacks);
  m_engine->sAppName = appName;

  const auto c = m_engine->Construct(dims.x,
                                     dims.y,
                                     enginePixToScreenPixRatio.x,
                                     enginePixToScreenPixRatio.y);
  if (c != olc::OK)
  {
    error("Could not build application", "Initialization failed");
  }
}

bool PGEApp::onCreate()
{
  m_debugLayer = 0u;

  m_uiLayer = m_engine->CreateLayer();
  m_engine->EnableLayer(m_uiLayer, true);

  m_drawLayer = m_engine->CreateLayer();
  m_engine->EnableLayer(m_drawLayer, true);

  m_decalLayer = m_engine->CreateLayer();
  m_engine->EnableLayer(m_decalLayer, true);

  m_renderer = std::make_unique<Renderer>(m_engine.get());

  const Vec2i screenDims{m_engine->ScreenWidth(), m_engine->ScreenHeight()};
  loadResources(screenDims, *m_renderer);

  return true;
}

namespace {
void setupLayerForRendering(olc::PixelGameEngine &engine)
{
  engine.SetPixelMode(olc::Pixel::ALPHA);
  engine.Clear(colors::toOlcPixel(colors::TRANSPARENT_WHITE));
}

void finalizeLayerRendering(olc::PixelGameEngine &engine)
{
  engine.SetPixelMode(olc::Pixel::NORMAL);
}

using Draw = std::function<void(RenderState &)>;

void renderLayer(olc::PixelGameEngine &engine,
                 const uint8_t layer,
                 RenderState &renderState,
                 const std::optional<Draw> &drawCall)
{
  engine.SetDrawTarget(layer);
  setupLayerForRendering(engine);
  if (drawCall)
  {
    (*drawCall)(renderState);
  }
  finalizeLayerRendering(engine);
}
} // namespace

bool PGEApp::onUpdate(const float elapsedSeconds)
{
  const auto ic = handleInputs();
  onInputs(m_controls, *m_frame);
  const auto quit = onFrame(elapsedSeconds);

  auto *baseDrawTarget = m_engine->GetDrawTarget();

  RenderState state{.frame = *m_frame, .renderer = *m_renderer};

  renderLayer(*m_engine, m_decalLayer, state, [this](RenderState &state) { drawDecal(state); });
  renderLayer(*m_engine, m_drawLayer, state, [this](RenderState &state) { draw(state); });
  if (hasUi())
  {
    renderLayer(*m_engine, m_uiLayer, state, [this](RenderState &state) { drawUi(state); });
  }
  else if (isFirstFrame())
  {
    renderLayer(*m_engine, m_uiLayer, state, {});
  }
  if (hasDebug())
  {
    Vec2f mPos(m_controls.mPosX, m_controls.mPosY);
    renderLayer(*m_engine, m_debugLayer, state, [this, &mPos](RenderState &state) {
      drawDebug(state, mPos);
    });
  }
  else if (ic.debugLayerToggled || isFirstFrame())
  {
    renderLayer(*m_engine, m_debugLayer, state, {});
  }

  m_engine->SetDrawTarget(baseDrawTarget);

  m_first = false;

  if (m_fpsControl)
  {
    m_fpsControl->throttle();
  }

  return !ic.quit && !quit;
}

bool PGEApp::onDestroy()
{
  cleanResources();
  m_renderer.reset();
  return true;
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

auto PGEApp::handleInputs() -> InputChanges
{
  InputChanges ic{};

  updateControls(m_engine.get(), m_controls);

  if (m_controls.released(controls::keys::ESCAPE))
  {
    ic.quit = true;
    return ic;
  }

  Vec2f mPos(m_engine->GetMousePos().x, m_engine->GetMousePos().y);

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
    int scroll = m_engine->GetMouseWheel();
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
