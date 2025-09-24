
#pragma once

#include "Unit.hh"
#include <string>

namespace chrono {

struct Duration
{
  Unit unit{Unit::SECONDS};
  float elapsed{0.0f};

  auto str() const -> std::string;
  auto convert(const Unit out) const -> Duration;
};

} // namespace chrono
