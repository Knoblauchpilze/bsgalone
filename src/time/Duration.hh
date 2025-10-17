
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

  static auto fromSeconds(const float duration) -> Duration;
  static auto fromMilliseconds(const float duration) -> Duration;
};

} // namespace chrono
