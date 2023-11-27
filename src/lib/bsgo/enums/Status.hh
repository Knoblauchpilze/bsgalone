
#pragma once

#include <string>

namespace bsgo {

enum class Status
{
  VISIBLE,
  APPEARING,
  DOCKED,
  THREAT,
  DEAD
};

auto str(const Status &status) -> std::string;

bool statusAllowsInteratction(const Status &status);
bool statusVisibleFromDradis(const Status &status);
bool statusAllowsRegeneration(const Status &status);
bool statusRequiresImmobilization(const Status &status);
bool statusIndicatesThreat(const Status &status);
bool statusIndicatesAppearing(const Status &status);

auto updateStatusWithThreat(const Status &in) -> Status;
auto updateStatusAfterSpawn(const Status &in) -> Status;
auto updateStatusAfterThreatEnded(const Status &in) -> Status;

} // namespace bsgo
