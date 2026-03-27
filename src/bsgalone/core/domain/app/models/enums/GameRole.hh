
#pragma once

#include <string>

namespace bsgalone::core {

enum class GameRole
{
  PILOT,
  GUNNER
};

auto str(const GameRole role) -> std::string;
auto toDbGameRole(const GameRole role) -> std::string;
auto fromDbGameRole(const std::string &dbGameRole) -> GameRole;

} // namespace bsgalone::core
