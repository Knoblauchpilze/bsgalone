
#pragma once

#include <string>

namespace bsgo {

enum class Status
{
  VISIBLE,
  APPEARING,
  DOCKED,
  DEAD
};

auto str(const Status &status) -> std::string;

} // namespace bsgo
