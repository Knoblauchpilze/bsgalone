
#pragma once

#include "Uuid.hh"

namespace bsgalone::core {

struct ResourceComponent
{
  Uuid resource{};
  int amount{};
};

} // namespace bsgalone::core
