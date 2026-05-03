
#pragma once

#include "Uuid.hh"

namespace bsgalone::core {

struct Loot
{
  Uuid resource{};
  int amount{};

  bool operator==(const Loot &rhs) const = default;
};

} // namespace bsgalone::core
