
#pragma once

#include "Tick.hh"
#include "TickDuration.hh"

namespace chrono {

struct TickData
{
  Tick tick{};
  TickDuration elapsed{};
};

} // namespace chrono
