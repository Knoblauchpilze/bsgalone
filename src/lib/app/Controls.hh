
#pragma once

#include <vector>

namespace pge::controls {

namespace keys {

/// @brief - The list of keys that are trackes by the
/// controls structure.
enum Keys
{
  RIGHT,
  D,
  UP,
  Z,
  LEFT,
  Q,
  DOWN,
  S,

  SPACE,

  P,
  M,

  KEYS_COUNT
};

} // namespace keys

namespace mouse {

/// @brief - The possible mouse buttons recognized for
/// controls.
enum MouseButton
{
  LEFT,
  MIDDLE,
  RIGHT,

  BUTTONS_COUNT
};

} // namespace mouse

/// @brief - The possible states for a button. Note
/// that the `PRESSED` state state is only active
/// once when the button is first pressed. Similarly
/// the `RELEASED` state is only active once when
/// the button is first released.
enum class ButtonState
{
  FREE,
  RELEASED,
  PRESSED,
  HELD
};

/// @brief - Describe a structure holding the controls
/// that are relevant extracted from the keys pressed
/// by the user and the mouse info.
struct State
{
  // The position of the mouse along the x coordinates.
  int mPosX{0};

  // The position of the mouse along the y coordinates.
  int mPosY{0};

  // The current state of the keys.
  std::vector<bool> keys;

  // The current state of the mouse buttons.
  std::vector<ButtonState> buttons;

  // Whether the tab key is pressed.
  bool tab{false};
};

/// @brief - Create a new controls structure.
/// @return - the created structure.
State newState() noexcept;

} // namespace pge::controls

#include "Controls.hxx"
