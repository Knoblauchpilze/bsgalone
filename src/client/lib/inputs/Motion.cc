
#include "Motion.hh"

namespace pge {

void Motion::updateFromKeys(const controls::State &inputs)
{
  if (inputs.held(controls::keys::LEFT) || inputs.held(controls::keys::Q))
  {
    --x;
  }
  if (inputs.held(controls::keys::RIGHT) || (inputs.held(controls::keys::D) && !inputs.shift))
  {
    ++x;
  }
  if (inputs.held(controls::keys::UP) || inputs.held(controls::keys::Z))
  {
    ++y;
  }
  if (inputs.held(controls::keys::DOWN) || inputs.held(controls::keys::S))
  {
    --y;
  }
}

} // namespace pge
