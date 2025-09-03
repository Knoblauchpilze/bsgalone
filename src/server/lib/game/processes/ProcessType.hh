
#pragma once

#include <string>

namespace bsgo {

enum class ProcessType
{
  DB_SYNC,
  RESPAWN
};

auto str(const ProcessType &type) -> std::string;

} // namespace bsgo
