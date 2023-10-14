
#pragma once

#include <string>

namespace bsgo {

enum class Faction
{
  COLONIAL,
  CYLON
};

auto str(const Faction faction) -> std::string;

} // namespace bsgo
