
#pragma once

#include <vector>

namespace pge::controls {

namespace keys {
enum Keys
{
  NONE,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  K0,
  K1,
  K2,
  K3,
  K4,
  K5,
  K6,
  K7,
  K8,
  K9,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  SPACE,
  TAB,
  SHIFT,
  CTRL,
  INS,
  DEL,
  HOME,
  END,
  PGUP,
  PGDN,
  BACK,
  ESCAPE,
  RETURN,
  ENTER,
  PAUSE,
  SCROLL,
  NP0,
  NP1,
  NP2,
  NP3,
  NP4,
  NP5,
  NP6,
  NP7,
  NP8,
  NP9,
  NP_MUL,
  NP_DIV,
  NP_ADD,
  NP_SUB,
  NP_DECIMAL,
  PERIOD,
  KEYS_COUNT
};

enum State
{
  FREE,
  RELEASED,
  PRESSED,
  HELD
};
} // namespace keys

namespace mouse {
enum Button
{
  LEFT = 0,
  MIDDLE,
  RIGHT,

  BUTTONS_COUNT
};

enum State
{
  FREE = 0,
  RELEASED,
  PRESSED,
  HELD
};
} // namespace mouse

/// @brief - Describe a structure holding the controls
/// that are relevant extracted from the keys pressed
/// by the user and the mouse info.
struct State
{
  int mPosX{0};
  int mPosY{0};

  std::vector<keys::State> keys{keys::KEYS_COUNT, keys::FREE};
  std::vector<mouse::State> buttons{mouse::BUTTONS_COUNT, mouse::FREE};

  bool tab{false};
  bool shift{false};

  bool clicked(const mouse::Button &button) const;
  bool pressed(const keys::Keys &key) const;
  bool held(const keys::Keys &key) const;
  bool released(const keys::Keys &key) const;
};

} // namespace pge::controls
