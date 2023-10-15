
#include "Power.hh"

namespace bsgo {

Power::Power(const float current, const float max, const float regen)
  : RegenerativeComponent("power", 0.0f, current, max, regen)
{}

} // namespace bsgo
