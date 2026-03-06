
#pragma once

#include "TickDuration.hh"
#include "TimeStep.hh"
#include "TimeUtils.hh"

namespace bsgalone::client {

auto convertTickToDuration(const chrono::TickDuration &elapsed, const chrono::TimeStep &timeStep)
  -> ::core::Duration;

}
