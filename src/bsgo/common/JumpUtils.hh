
#pragma once

#include "PlayerShipRepository.hh"
#include <string>

namespace bsgo {

bool isShipActive(const PlayerShip &ship);

enum class JumpCompletionStatus
{
  SHIP_NOT_ACTIVE,
  JUMP_SYSTEM_NOT_SET,
  SHIP_SYSTEM_NOT_SET,
  JUMPING_TO_SAME_SYSTEM,
  SHIP_IS_DOCKED,
  OK
};
auto str(const JumpCompletionStatus status) -> std::string;
auto canShipCompleteJump(const PlayerShip &ship) -> JumpCompletionStatus;

} // namespace bsgo
