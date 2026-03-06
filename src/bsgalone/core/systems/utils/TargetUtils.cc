
#include "SystemUtils.hh"

namespace bsgalone::core {

bool canTargetBeFiredOn(const Entity &target)
{
  if (!target.valid())
  {
    return false;
  }
  if (!target.exists<StatusComponent>())
  {
    return true;
  }

  const auto status = target.statusComp().status();
  return statusAllowsInteraction(status) && statusAllowsDamage(status);
}

} // namespace bsgalone::core
