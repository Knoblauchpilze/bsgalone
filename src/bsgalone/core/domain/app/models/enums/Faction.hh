
#pragma once

#include <string>

namespace bsgalone::core {

enum class Faction
{
  COLONIAL,
  CYLON
};

auto str(const Faction faction) -> std::string;
auto toDbFaction(const Faction faction) -> std::string;
auto fromDbFaction(const std::string &dbFaction) -> Faction;

} // namespace bsgalone::core
