
#pragma once

#include <string>

namespace bsgo {

enum class Slot
{
  WEAPON,
};

auto str(const Slot slot) -> std::string;

} // namespace bsgo
