
#pragma once

#include "SystemDataMessage.hh"
#include <vector>

namespace test {

auto generateSystemDataMessage(std::vector<bsgalone::core::Asteroid> asteroids)
  -> bsgalone::core::SystemDataMessage;

} // namespace test
