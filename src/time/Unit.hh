
#pragma once

#include <string>

namespace chrono {

enum class Unit
{
  MILLISECONDS,
  SECONDS,
};

auto str(const Unit unit) -> std::string;
auto asTimeString(const Unit unit) -> std::string;

} // namespace chrono
