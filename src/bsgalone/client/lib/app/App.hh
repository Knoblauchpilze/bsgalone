
#pragma once

#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiCommandQueue.hh"
#include "IUiHandler.hh"
#include "PGEApp.hh"
#include "Screen.hh"
#include <unordered_map>

namespace bsgalone::client {

class App : public pge::PGEApp
{
  public:
  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  App(const pge::AppDesc &desc);

  ~App() override = default;

  void onScreenChanged(const Screen screen);

  protected:
  bool onFrame(const float elapsedSeconds) override;
  void onInputs(const pge::controls::State &controls, pge::CoordinateFrame &frame) override;

  void loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine) override;
  void cleanResources() override;

  void drawDecal(const pge::RenderState &state) override;
  void draw(const pge::RenderState &state) override;
  void drawUi(const pge::RenderState &state) override;
  void drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos) override;

  private:
  /// @brief - Defines the current screen selected in this game. Updated when
  /// the user takes action to change it.
  Screen m_screen{Screen::LOGIN};

  /// @brief - Holds the renderers associated to each screen. The app guarantees
  /// calling each renderer once per frame when the corresponding screen is active.
  std::unordered_map<Screen, IRendererPtr> m_renderers{};

  /// @brief - Holds the UI handlers associated to each screen. The app guarantees
  /// calling each handler once per frame when the corresponding screen is active.
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  /// @brief - Holds the input handlers associated to each screen. The app guarantees
  /// calling each handler once per frame when the corresponding screen is active.
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};

  /// @brief - Used by the UI components to trigger updates based on actions
  /// taken by the user (e.g. clicks or buttons). Some actions may result in
  /// commands sent to the server while others will trigger internal changes
  /// in the UI or client application in general.
  IUiCommandQueueShPtr m_uiCommandQueue{};

  void initializeMessageSystem();

  void generateUiHandlers(const pge::Vec2i &screenDims, pge::sprites::TexturePack &texturesLoader);
  void generateRenderers(const pge::Vec2i &dimensions, pge::sprites::TexturePack &texturesLoader);
};

} // namespace bsgalone::client
