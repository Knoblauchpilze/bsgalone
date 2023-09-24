
#pragma once

#include "Controls.hh"

namespace pge::controls {

inline State newState() noexcept
{
  State c;

  c.mPosX = 0;
  c.mPosY = 0;

  c.keys.resize(keys::KeysCount, false);
  c.buttons.resize(mouse::ButtonsCount, ButtonState::Free);

  c.tab = false;

  return c;
}

} // namespace pge::controls
