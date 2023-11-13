
#pragma once

#include <string>

namespace bsgo {

enum class Faction
{
  COLONIAL,
  CYLON
};

auto str(const Faction faction) -> std::string;
auto toDbFaction(const Faction faction) -> std::string;
auto fromDbFaction(const std::string &dbFaction) -> Faction;

} // namespace bsgo
