
#pragma once

#include "Controls.hh"

namespace pge::controls {

inline State newState() noexcept
{
  State c;

  c.keys.resize(keys::KEYS_COUNT, false);
  c.buttons.resize(mouse::BUTTONS_COUNT, ButtonState::FREE);

  return c;
}

} // namespace pge::controls
