
#include "Motion.hh"

namespace pge {

void Motion::updateFromKeys(const controls::State &inputs)
{
  if (inputs.keys[controls::keys::LEFT] || inputs.keys[controls::keys::Q])
  {
    --x;
  }
  if (inputs.keys[controls::keys::RIGHT] || (inputs.keys[controls::keys::D] && !inputs.shift))
  {
    ++x;
  }
  if (inputs.keys[controls::keys::UP] || inputs.keys[controls::keys::Z])
  {
    ++y;
  }
  if (inputs.keys[controls::keys::DOWN] || inputs.keys[controls::keys::S])
  {
    --y;
  }
}

} // namespace pge
