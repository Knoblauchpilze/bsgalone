
#include "Controls.hh"

namespace pge::controls {

bool State::clicked(const mouse::Button &button) const
{
  return mouse::RELEASED == buttons[button];
}

bool State::pressed(const keys::Keys &key) const
{
  return keys::PRESSED == keys[key];
}

bool State::held(const keys::Keys &key) const
{
  return keys::PRESSED == keys[key] || keys::HELD == keys[key];
}

bool State::released(const keys::Keys &key) const
{
  return keys::RELEASED == keys[key];
}

} // namespace pge::controls
