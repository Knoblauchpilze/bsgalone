
#pragma once

#include "Controls.hh"
#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "GameNetworkClient.hh"
#include "GameRole.hh"
#include "GameSession.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "LoadingTransition.hh"
#include "RenderState.hh"
#include "Renderer.hh"
#include "RenderingPass.hh"
#include "Screen.hh"
#include "SystemTickData.hh"
#include "TimeManager.hh"
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
  Game(const int serverPort,
       const std::optional<std::string> &userName,
       const std::optional<std::string> &password,
       const std::optional<bsgo::GameRole> &role);
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

  void onConnectedToServer();
  void onLogin(const bsgo::Uuid playerDbId, const bsgo::GameRole role);
  void onLoginDataReceived(const bsgo::Uuid playerShipDbId,
                           const bsgo::Uuid systemDbId,
                           const bsgo::Faction faction);
  void onLogout();
  void onActiveShipChanged(const bsgo::Uuid shipDbId);
  void onActiveSystemChanged(const bsgo::Uuid systemDbId);
  void onShipDocked();
  void onShipUndocked();
  void onPlayerKilled();
  void onSystemDataReceived(const bsgo::SystemTickData &systemData);
  void onLoadingStarted(const bsgo::LoadingTransition transition);
  void onLoadingFinished(const bsgo::LoadingTransition transition);

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

  /// @brief - Holds the user name to use to login automatically upon connecting
  /// to the server. If this vaule is empty the login/signup screen will stay
  /// visible for the user to enter their credentials.
  /// In case it is defined, the game will try to login automatically using the
  /// provided user name and password.
  std::optional<std::string> m_userName{};

  /// @brief - Holds the password to use to login automatically upon connecting
  /// to the server.
  std::optional<std::string> m_password{};

  /// @brief - Holds the game role to use to login automatically upon connecting
  /// to the server.
  std::optional<bsgo::GameRole> m_gameRole{};

  GameNetworkClientShPtr m_networkClient{};

  /// @brief - Holds information about the current game session. This includes
  /// data about the current player, their ship, the system they are in, etc.
  GameSessionShPtr m_gameSession{std::make_shared<GameSession>()};

  bsgo::DatabaseEntityMapper m_entityMapper{};
  chrono::TimeManagerPtr m_timeManager{};
  bsgo::CoordinatorShPtr m_coordinator{};
  bsgalone::core::IMessageQueuePtr m_internalMessageQueue{};
  Views m_views{};
  std::unordered_map<Screen, IRendererPtr> m_renderers{};
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  void initialize(const int serverPort);
  void initializeMessageSystem();
};

using GameShPtr = std::shared_ptr<Game>;
} // namespace pge
