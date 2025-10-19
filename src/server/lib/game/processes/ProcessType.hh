
#pragma once

#include <string>

namespace bsgo {

enum class ProcessType
{
  DB_SYNC,
  RESPAWN,
  TICK_SYNC,
};

auto str(const ProcessType &type) -> std::string;

} // namespace bsgo
