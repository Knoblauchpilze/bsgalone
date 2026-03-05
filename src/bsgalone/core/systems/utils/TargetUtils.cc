
#include "SystemUtils.hh"

namespace bsgo {

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

} // namespace bsgo
