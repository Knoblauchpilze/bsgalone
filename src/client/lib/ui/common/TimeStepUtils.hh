
#pragma once

#include "TickDuration.hh"
#include "TimeStep.hh"

namespace pge {

auto convertTickToMilliseconds(const chrono::TickDuration &elapsed,
                               const chrono::TimeStep &timeStep) -> float;

}
