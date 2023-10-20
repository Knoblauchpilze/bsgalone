
#include "PowerComponent.hh"

namespace bsgo {

PowerComponent::PowerComponent(const float current, const float max, const float regen)
  : RegenerativeComponent("power", 0.0f, current, max, regen)
{}

void PowerComponent::usePower(const float power)
{
  updateValue(-power);
}

} // namespace bsgo
