
#include "Health.hh"

namespace bsgo {

Health::Health(const float current, const float max, const float regen)
  : RegenerativeComponent("health", 0.0f, current, max, regen)
{}

} // namespace bsgo
