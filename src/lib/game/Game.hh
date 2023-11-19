
#pragma once

#include "Action.hh"
#include "Controls.hh"
#include "DataSource.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "RenderState.hh"
#include "RenderingPass.hh"
#include "Screen.hh"
#include "SpriteRenderer.hh"
#include "Views.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <unordered_map>

namespace pge {

// Forward declaration of the `Menu` class to be able
// to use it right away.
class Menu;
using MenuShPtr = std::shared_ptr<Menu>;

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

  /// @brief - Retrieves the currently selected screen.
  /// @return - the currently selected screen.
  auto getScreen() const noexcept -> Screen;

  /// @brief - Define a new active screen for this game.
  /// @param screen - the new screen to apply.
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

  private:
  /// @brief - Convenience structure allowing to group information
  /// about a timed menu.
  struct TimedMenu
  {
    // Information about when the menu started appearing.
    utils::TimeStamp date;

    // Keep track of whether the menu was already active.
    bool wasActive;

    // The alert menu indicating controlled by this object.
    MenuShPtr menu;

    // The duration of the alert.
    int duration;

    /// @brief - Used to update the internal attribute with the current value
    /// of whether the menu should be active or not.
    /// @param active - `true` if the menu should still be active.
    /// @return - `true` if the menu is still visible.
    bool update(bool active) noexcept;
  };

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
  bsgo::CoordinatorShPtr m_coordinator{nullptr};
  bsgo::Views m_views;
  std::unordered_map<Screen, IRendererPtr> m_renderers{};
  std::unordered_map<Screen, IInputHandlerPtr> m_inputHandlers{};
  std::unordered_map<Screen, IUiHandlerPtr> m_uiHandlers{};

  void initialize();
};

using GameShPtr = std::shared_ptr<Game>;
} // namespace pge
