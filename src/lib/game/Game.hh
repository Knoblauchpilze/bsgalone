
#pragma once

#include "Controls.hh"
#include "DataSource.hh"
#include "GameMessageModule.hh"
#include "IInputHandler.hh"
#include "IMessageListener.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "MessageQueue.hh"
#include "NetworkSystem.hh"
#include "RenderState.hh"
#include "RenderingPass.hh"
#include "Screen.hh"
#include "Services.hh"
#include "SpriteRenderer.hh"
#include "Views.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <unordered_map>

namespace pge {

class IRenderer;
using IRendererPtr = std::unique_ptr<IRenderer>;

class IUiHandler;
using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

class IInputHandler;
using IInputHandlerPtr = std::unique_ptr<IInputHandler>;

class Game : public utils::CoreObject
{
  public:
  /// @brief - Create a new game with default parameters.
  Game();

  ~Game() = default;

  auto getScreen() const noexcept -> Screen;
  void setScreen(const Screen &screen);

  void generateRenderers(int width, int height, SpriteRenderer &spriteRenderer);
  void generateInputHandlers();
  void generateUiHandlers(int width, int height);

  void processUserInput(const controls::State &controls, CoordinateFrame &frame);
  void render(SpriteRenderer &engine, const RenderState &state, const RenderingPass pass) const;

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

  void tryActivateWeapon(const bsgo::Uuid &ship, const int &weaponId);
  void tryActivateSlot(const bsgo::Uuid &ship, const int &slotId);
  void login(const bsgo::Uuid &playerDbId);
  void requestJump(const bsgo::Uuid &systemId);
  void activeShipChanged();
  void activeSystemChanged();

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

    // Used to hold whether or not the game has been shut
    // down. It usually indicates that no simulation will
    // be performed anymore and usually indicates that a
    // termination request has been received.
    bool terminated{false};
  };

  /// @brief - The definition of the game state.
  State m_state{};

  bsgo::DataSource m_dataSource{};
  bsgo::Services m_services{};
  bsgo::CoordinatorShPtr m_coordinator{};
  bsgo::IMessageQueuePtr m_messageQueue{};
  bsgo::NetworkSystem *m_networkSystem{};
  bsgo::Views m_views;
  std::unordered_map<Screen, IRendererPtr> m_renderers{};
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};
  std::vector<bsgo::IMessageListenerPtr> m_messageConsumers{};
  GameMessageModule m_messageModule;

  void initialize();
  void resetViewsAndUi();
};

using GameShPtr = std::shared_ptr<Game>;
} // namespace pge
