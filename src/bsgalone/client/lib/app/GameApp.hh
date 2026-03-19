
#pragma once

#include "GameNetworkClient.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "PGEApp.hh"
#include "Screen.hh"
#include "ServerConfig.hh"
#include "SynchronizedUiEventQueue.hh"
#include <unordered_map>

namespace bsgalone::client {

// TODO: This is currently unused but is meant to replace the App class.
class GameApp : public pge::PGEApp
{
  public:
  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param config - description of the configuration to connect to the server and
  /// possibly auto connect to it with a specified user.
  GameApp(const pge::AppDesc &desc, ServerConfig config);

  ~GameApp() override = default;

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
  /// @brief - The port the server is listening on. Used when attempting to connect
  /// to the remote server.
  int m_serverPort{};

  /// @brief - Defines the current screen selected in this game. Updated whenever
  /// the user takes action to change it.
  Screen m_screen{Screen::LOGIN};

  /// @brief - The network client used to connect to the server and transmit
  /// commands and receive updates to the game.
  GameNetworkClientShPtr m_networkClient{};

  /// @brief - Used by the UI components to trigger updates based on actions
  /// taken by the user (e.g. clicks or buttons). Some actions may result in
  /// commands sent to the server while others will trigger internal changes
  /// in the UI or client application in general.
  IUiEventQueueShPtr m_uiCommandQueue{createSynchronizedUiEventQueue()};

  /// @brief - A map holding the renderers attached to each screen. The renderer
  /// is guaranteed to be triggered when the screen is active.
  std::unordered_map<Screen, IRendererPtr> m_renderers{};

  /// @brief - A map holding the UI handlers attached to each screen. The renderer
  /// is guaranteed to be triggered when the screen is active.
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  void generateRenderers(const pge::Vec2i &screenDims, pge::Renderer &engine);
  void generateUiHandlers(const pge::Vec2i &screenDims, pge::Renderer &engine);
  void initializeMessageSystem();
};

} // namespace bsgalone::client
