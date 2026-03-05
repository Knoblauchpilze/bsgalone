
#include "JumpUtils.hh"

namespace bsgo {

bool isShipActive(const PlayerShip &ship)
{
  return ship.active;
}

auto str(const JumpCompletionStatus status) -> std::string
{
  switch (status)
  {
    case JumpCompletionStatus::JUMP_SYSTEM_NOT_SET:
      return "jump_system_not_set";
    case JumpCompletionStatus::JUMP_SYSTEM_ALREADY_SET:
      return "jump_system_already_set";
    case JumpCompletionStatus::JUMPING_TO_SAME_SYSTEM:
      return "jumping_to_same_system";
    case JumpCompletionStatus::SHIP_IS_DOCKED:
      return "ship_is_docked";
    case JumpCompletionStatus::SHIP_NOT_ACTIVE:
      return "ship_not_active";
    case JumpCompletionStatus::SHIP_SYSTEM_NOT_SET:
      return "ship_system_not_set";
    case JumpCompletionStatus::OK:
      return "ok";
    default:
      return "unknown";
  }
}

auto canShipJump(const PlayerShip &ship, const Uuid newSystem) -> JumpCompletionStatus
{
  if (!isShipActive(ship))
  {
    return JumpCompletionStatus::SHIP_NOT_ACTIVE;
  }
  if (!ship.system.has_value())
  {
    return JumpCompletionStatus::SHIP_SYSTEM_NOT_SET;
  }
  if (ship.jumpSystem.has_value())
  {
    return JumpCompletionStatus::JUMP_SYSTEM_ALREADY_SET;
  }
  if (*ship.system == newSystem)
  {
    return JumpCompletionStatus::JUMPING_TO_SAME_SYSTEM;
  }
  if (ship.docked)
  {
    return JumpCompletionStatus::SHIP_IS_DOCKED;
  }

  return JumpCompletionStatus::OK;
}

auto canShipCompleteJump(const PlayerShip &ship) -> JumpCompletionStatus
{
  if (!isShipActive(ship))
  {
    return JumpCompletionStatus::SHIP_NOT_ACTIVE;
  }
  if (!ship.system.has_value())
  {
    return JumpCompletionStatus::SHIP_SYSTEM_NOT_SET;
  }
  if (!ship.jumpSystem.has_value())
  {
    return JumpCompletionStatus::JUMP_SYSTEM_NOT_SET;
  }
  if (*ship.jumpSystem == *ship.system)
  {
    return JumpCompletionStatus::JUMPING_TO_SAME_SYSTEM;
  }
  if (ship.docked)
  {
    return JumpCompletionStatus::SHIP_IS_DOCKED;
  }

  return JumpCompletionStatus::OK;
}

auto canShipCancelJump(const PlayerShip &ship) -> JumpCompletionStatus
{
  if (!isShipActive(ship))
  {
    return JumpCompletionStatus::SHIP_NOT_ACTIVE;
  }
  if (!ship.system.has_value())
  {
    return JumpCompletionStatus::SHIP_SYSTEM_NOT_SET;
  }
  if (!ship.jumpSystem.has_value())
  {
    return JumpCompletionStatus::JUMP_SYSTEM_NOT_SET;
  }
  if (ship.docked)
  {
    return JumpCompletionStatus::SHIP_IS_DOCKED;
  }

  return JumpCompletionStatus::OK;
}

} // namespace bsgo
