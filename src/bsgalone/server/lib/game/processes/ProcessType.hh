
#pragma once

#include <string>

namespace bsgalone::server {

enum class ProcessType
{
  DB_SYNC,
  RESPAWN,
  TICK_SYNC,
};

auto str(const ProcessType &type) -> std::string;

} // namespace bsgalone::server
