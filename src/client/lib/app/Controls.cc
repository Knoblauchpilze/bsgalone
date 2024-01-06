
#include "Controls.hh"

namespace pge::controls {

bool State::pressed(const mouse::Button &button) const
{
  return mouse::PRESSED == buttons[button];
}

bool State::held(const mouse::Button &button) const
{
  return mouse::HELD == buttons[button];
}

bool State::released(const mouse::Button &button) const
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
