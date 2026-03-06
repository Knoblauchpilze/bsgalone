
#include "Motion.hh"

namespace bsgalone::client {

void Motion::updateFromKeys(const pge::controls::State &inputs)
{
  if (inputs.held(pge::controls::keys::LEFT) || inputs.held(pge::controls::keys::Q))
  {
    --x;
  }
  if (inputs.held(pge::controls::keys::RIGHT)
      || (inputs.held(pge::controls::keys::D) && !inputs.shift))
  {
    ++x;
  }
  if (inputs.held(pge::controls::keys::UP) || inputs.held(pge::controls::keys::Z))
  {
    ++y;
  }
  if (inputs.held(pge::controls::keys::DOWN) || inputs.held(pge::controls::keys::S))
  {
    --y;
  }
}

bool Motion::hasMotion() const
{
  return x != 0 || y != 0 || z != 0;
}

} // namespace bsgalone::client
