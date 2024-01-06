
#pragma once

#include <string>

namespace bsgo {

enum class Slot
{
  WEAPON,
  COMPUTER,
};

auto str(const Slot slot) -> std::string;
auto toDbSlot(const Slot slot) -> std::string;
auto fromDbSlot(const std::string &dbSlot) -> Slot;

} // namespace bsgo
