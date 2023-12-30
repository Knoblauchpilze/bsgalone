
#include "JumpService.hh"

namespace bsgo {

JumpService::JumpService(const Repositories &repositories)
  : AbstractService("jump", repositories)
{}

bool JumpService::tryRegisterJump(const Uuid &shipId, const Uuid &system) const
{
  warn("should try to register jump for " + str(shipId) + " to " + str(system));
  return true;
}

bool JumpService::tryCancelJump(const Uuid &shipId) const
{
  warn("should try to cancel jump for " + str(shipId));
  return true;
}

bool JumpService::tryJump(const Uuid &shipId) const
{
  warn("should try to jump " + str(shipId));
  return true;
}

} // namespace bsgo
