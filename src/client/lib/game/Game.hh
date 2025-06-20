
#pragma once

#include "ClientMessageQueue.hh"
#include "Context.hh"
#include "Controls.hh"
#include "CoreObject.hh"
#include "DataSource.hh"
#include "DatabaseEntityMapper.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "RenderState.hh"
#include "Renderer.hh"
#include "RenderingPass.hh"
#include "Screen.hh"
#include "Views.hh"
#include <memory>
#include <unordered_map>

namespace pge {

class IRenderer;
using IRendererPtr = std::unique_ptr<IRenderer>;

class IUiHandler;
using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

class IInputHandler;
using IInputHandlerPtr = std::unique_ptr<IInputHandler>;

class Game : public core::CoreObject
{
  public:
  Game(const int serverPort);
  ~Game() override;

  auto getScreen() const noexcept -> Screen;
  void setScreen(const Screen &screen);

  void generateRenderers(int width, int height, Renderer &engine);
  void generateInputHandlers();
  void generateUiHandlers(int width, int height, Renderer &engine);

  void processUserInput(const controls::State &controls, CoordinateFrame &frame);
  void render(const RenderState &state, const RenderingPass pass) const;

  /// @brief - Requests the game to be terminated. This is applied to the next
  /// iteration of the game loop.
  void terminate() noexcept;

  /// @brief - Returns whether or not the game has been terminated. The game is
  /// terminated when the user wants to exit the app (usually).
  bool terminated() const noexcept;

  /// @brief - Forward the call to step one step ahead in time to the internal world.
  /// @param elapsedSeconds - the duration of the last frame in seconds.
  /// @param bool - `true` in case the game continues, and `false` otherwise (i.e. if
  /// the game is ended).
  bool step(float elapsedSeconds);

  void onConnectedToServer(const bsgo::Uuid clientId);
  void onLogin(const bsgo::Uuid playerDbId);
  void onLogout();
  void onActiveShipChanged();
  void onActiveSystemChanged();
  void onShipDocked();
  void onShipUndocked();
  void onPlayerKilled();
  void onLoadingStarted();
  void onLoadingFinished();

  private:
  /// @brief - Convenience information defining the state of the
  /// game. It includes information about whether the menus should
  /// be displayed and if the user actions should be interpreted
  /// or not.
  struct State
  {
    /// @brief - Defines the current screen selected in this game. Updated whenever
    /// the user takes action to change it.
    Screen screen;

    /// @brief - Whether the game was terminated (usually because the app was closed).
    bool terminated{false};

    /// @brief - Whether the player is dead or not.
    bool dead{false};
  };

  /// @brief - The data associated with the current gaming session. This
  /// include information about the player (such as the current system,
  /// the ID of the player in the DB) but also about the game in general
  /// such as the next screen to be displayed when waiting for loading
  /// data to be communicated by the server.
  struct GameSession
  {
    std::optional<bsgo::Uuid> systemDbId{};
    std::optional<bsgo::Uuid> playerDbId{};

    /// @brief - holds the screen that was displayed before the current
    /// loading phase.
    std::optional<Screen> previousScreen{};

    /// @brief - holds the screen that should be displayed after the
    /// loading phase is finished.
    std::optional<Screen> nextScreen{};
  };

  /// @brief - The definition of the game state.
  State m_state{};

  GameSession m_gameSession{};

  bsgo::DataSource m_dataSource{bsgo::DataLoadingMode::CLIENT};
  bsgo::DatabaseEntityMapper m_entityMapper{};
  bsgo::CoordinatorShPtr m_coordinator{};
  bsgo::IMessageQueuePtr m_inputMessageQueue{};
  bsgo::IMessageQueuePtr m_internalMessageQueue{};
  ClientMessageQueuePtr m_outputMessageQueue{};
  net::ContextPtr m_networkContext{std::make_unique<net::Context>()};
  bsgo::Views m_views{};
  std::unordered_map<Screen, IRendererPtr> m_renderers{};
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  void initialize(const int serverPort);
  void initializeMessageSystem();

  void resetViewsAndUi();

  void setupLoadingScreen(const Screen nextScreen);
};

using GameShPtr = std::shared_ptr<Game>;
} // namespace pge
