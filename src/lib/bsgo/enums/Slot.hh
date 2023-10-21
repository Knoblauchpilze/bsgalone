
#pragma once

#include <string>

namespace bsgo {

enum class Slot
{
  WEAPON,
  COMPUTER,
};

auto str(const Slot slot) -> std::string;

} // namespace bsgo
