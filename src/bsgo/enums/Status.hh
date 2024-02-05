
#pragma once

#include <string>

namespace bsgo {

enum class Status
{
  VISIBLE,
  APPEARING,
  DOCKED,
  THREAT,
  JUMP,
  JUMP_APPEARING,
  JUMP_THREAT,
  DEAD
};

auto str(const Status &status) -> std::string;

bool statusVisibleFromDradis(const Status &status);
bool statusAllowsInteratction(const Status &status);
bool statusAllowsHealthRegeneration(const Status &status);
bool statusAllowsPowerRegeneration(const Status &status);
bool statusAllowsDamage(const Status &status);
bool statusRequiresImmobilization(const Status &status);
bool statusRequiresThreatReset(const Status &status);
bool statusRequiresPowerReset(const Status &status);
bool statusIndicatesThreat(const Status &status);
bool statusIndicatesAppearing(const Status &status);
bool statusIndicatesJump(const Status &status);

auto updateStatusWithThreat(const Status &in) -> Status;
auto updateStatusAfterSpawn(const Status &in) -> Status;
auto updateStatusAfterThreatEnded(const Status &in) -> Status;
auto updateStatusForJump(const Status &in) -> Status;
auto updateStatusAfterJumpCancellation(const Status &in) -> Status;

} // namespace bsgo
