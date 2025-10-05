
#pragma once

#include "Tick.hh"
#include "TickDuration.hh"

namespace bsgo {

struct TickData
{
  Tick tick{};
  TickDuration elapsed{};
};

} // namespace bsgo
