
#pragma once

#include "Game.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "PGEApp.hh"
#include "Screen.hh"
#include "ServerConfig.hh"
#include "SynchronizedUiEventQueue.hh"
#include <unordered_map>

namespace bsgalone::client {

class App : public pge::PGEApp
{
  public:
  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param config - description of the configuration to connect to the server and
  /// possibly auto connect to it with a specified user.
  App(const pge::AppDesc &desc, ServerConfig config);

  ~App() override = default;

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
  ServerConfig m_config{};
  GameShPtr m_game{nullptr};

  /// @brief - Defines the current screen selected in this game. Updated when
  /// the user takes action to change it.
  Screen m_screen{Screen::LOGIN};

  /// @brief - Used to publish incoming events that are relevant for the UI
  /// components. This is used to asynchronously notify the UI from changes
  /// to the game.
  IUiEventQueuePtr m_uiEventBus{createSynchronizedUiEventQueue()};

  /// @brief - Used by the UI components to trigger updates based on actions
  /// taken by the user (e.g. clicks or buttons). Some actions may result in
  /// commands sent to the server while others will trigger internal changes
  /// in the UI or client application in general.
  IUiEventQueuePtr m_uiCommandQueue{createSynchronizedUiEventQueue()};

  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  void initializeMessageSystem();
};

} // namespace bsgalone::client
