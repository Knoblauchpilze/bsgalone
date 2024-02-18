
#pragma once

#include "PlayerShipRepository.hh"
#include <string>

namespace bsgo {

bool isShipActive(const PlayerShip &ship);

enum class JumpCompletionStatus
{
  JUMP_SYSTEM_NOT_SET,
  JUMP_SYSTEM_ALREADY_SET,
  JUMPING_TO_SAME_SYSTEM,
  SHIP_IS_DOCKED,
  SHIP_NOT_ACTIVE,
  SHIP_SYSTEM_NOT_SET,
  OK
};
auto str(const JumpCompletionStatus status) -> std::string;

auto canShipJump(const PlayerShip &ship, const Uuid newSystem) -> JumpCompletionStatus;
auto canShipCompleteJump(const PlayerShip &ship) -> JumpCompletionStatus;
auto canShipCancelJump(const PlayerShip &ship) -> JumpCompletionStatus;

} // namespace bsgo
