
#pragma once

#include "Uuid.hh"

namespace bsgalone::core {

struct Loot
{
  Uuid resource{};
  int amount{};
};

} // namespace bsgalone::core
