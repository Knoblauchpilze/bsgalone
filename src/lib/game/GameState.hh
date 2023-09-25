
#pragma once

#include "Menu.hh"
#include "olcEngine.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

enum class Screen
{
  LOGIN,
  GAME,
  MAP,
  OUTPOST,
  GAMEOVER,
  EXIT
};

class GameState : public utils::CoreObject
{
  public:
  /// @brief - Creates a new game state with the specified state.
  /// @param dims - the dimensions of the desired UI.
  /// @param screen - the current screen.
  GameState(const olc::vi2d &dims, const Screen &screen);

  /// @brief - Destructor to disconnect the signal from the saved games object.
  ~GameState() override = default;

  /// @brief - Retrieves the currently selected screen.
  /// @return - the currently selected screen.
  auto getScreen() const noexcept -> Screen;

  /// @brief - Define a new active screen for this game.
  /// @param screen - the new screen to apply.
  void setScreen(const Screen &screen);

  /// @brief - Used to render the screen corresponding to the current state using
  /// the provided game renderer.
  /// @param pge - the engine to use to render the game state.
  void render(olc::PixelGameEngine *pge) const;

  /// @brief - Performs the interpretation of the controls provided as input to
  /// update the selected screen. Actions may be generated through this mechanism.
  /// @param c - the controls and user input for this frame.
  /// @param actions - the list of actions produced by the menu while processing
  /// the events.
  /// @return - the description of what happened when the inputs has been processed.
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle;

  private:
  void generateLoginScreen(const olc::vi2d &dims);
  void generateMapScreen(const olc::vi2d &dims);
  void generateOutpostScreen(const olc::vi2d &dims);
  void generateGameOverScreen(const olc::vi2d &dims);

  private:
  /// @brief - Defines the current screen selected in this game. Updated whenever
  /// the user takes action to change it.
  Screen m_screen;

  MenuShPtr m_login{nullptr};
  MenuShPtr m_map{nullptr};
  MenuShPtr m_outpost{nullptr};
  MenuShPtr m_gameOver{nullptr};
};

using GameStateShPtr = std::shared_ptr<GameState>;
} // namespace pge
