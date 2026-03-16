
#pragma once

#include "AbstractUiHandler.hh"
#include "Controls.hh"
#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "GameNetworkClient.hh"
#include "GameRole.hh"
#include "GameSession.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "LoadingTransition.hh"
#include "RenderState.hh"
#include "Renderer.hh"
#include "RenderingPass.hh"
#include "Screen.hh"
#include "ServerConfig.hh"
#include "TimeManager.hh"
#include "UserInputData.hh"
#include "Views.hh"
#include <memory>
#include <unordered_map>

namespace bsgalone::client {

class Game : public ui::IScreenChanger, public ::core::CoreObject
{
  public:
  Game(const ServerConfig &config);
  ~Game() override;

  auto getScreen() const noexcept -> Screen;
  void setScreen(const Screen screen) override;

  void generateRenderers(int width, int height, pge::Renderer &engine);
  void generateInputHandlers();
  void generateUiHandlers(int width, int height, pge::Renderer &engine);

  void processUserInput(const pge::controls::State &controls, pge::CoordinateFrame &frame);
  void render(const pge::RenderState &state, const pge::RenderingPass pass) const;

  /// @brief - Requests the game to be terminated. This is applied to the next
  /// iteration of the game loop.
  void terminate() noexcept override;

  /// @brief - Returns whether or not the game has been terminated. The game is
  /// terminated when the user wants to exit the app (usually).
  bool terminated() const noexcept;

  /// @brief - Forward the call to step one step ahead in time to the internal world.
  /// @param elapsedSeconds - the duration of the last frame in seconds.
  /// @param bool - `true` in case the game continues, and `false` otherwise (i.e. if
  /// the game is ended).
  bool step(float elapsedSeconds);

  void onLogin(const core::Uuid playerDbId, const core::GameRole role);
  void onLoginDataReceived(const core::Uuid playerShipDbId,
                           const core::Uuid systemDbId,
                           const core::Faction faction);
  void onLogout();
  void onActiveShipChanged(const core::Uuid shipDbId);
  void onActiveSystemChanged(const core::Uuid systemDbId);
  void onShipDocked();
  void onShipUndocked();
  void onPlayerKilled();
  void onSystemListReceived(const std::vector<core::System> &systemsData);
  void onLoadingStarted(const core::LoadingTransition transition);
  void onLoadingFinished(const core::LoadingTransition transition);

  private:
  /// @brief - Convenience information defining the state of the
  /// game. It includes information about whether the menus should
  /// be displayed and if the user actions should be interpreted
  /// or not.
  struct State
  {
    /// @brief - Defines the current screen selected in this game. Updated whenever
    /// the user takes action to change it.
    Screen screen{Screen::LOGIN};

    /// @brief - Whether the game was terminated (usually because the app was closed).
    bool terminated{false};

    /// @brief - Whether the player is dead or not.
    bool dead{false};
  };

  /// @brief - The definition of the game state.
  State m_state{};

  GameNetworkClientShPtr m_networkClient{};

  /// @brief - Holds information about the current game session. This includes
  /// data about the current player, their ship, the system they are in, etc.
  GameSessionShPtr m_gameSession{std::make_shared<GameSession>()};

  core::DatabaseEntityMapper m_entityMapper{};
  chrono::TimeManagerPtr m_timeManager{};
  core::CoordinatorShPtr m_coordinator{};
  core::IMessageQueuePtr m_internalMessageQueue{};
  Views m_views{};
  std::unordered_map<Screen, IRendererPtr> m_renderers{};
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};
  std::unordered_map<Screen, AbstractUiHandlerPtr> m_uiHandlers{};

  void initialize(const ServerConfig &config);
  void initializeMessageSystem();
};

using GameShPtr = std::shared_ptr<Game>;
} // namespace bsgalone::client
