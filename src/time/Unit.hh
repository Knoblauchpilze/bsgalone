
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

auto fromString(const std::string_view unit) -> Unit;

} // namespace chrono
