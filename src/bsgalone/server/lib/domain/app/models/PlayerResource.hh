
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgalone::server {

struct PlayerResource
{
  core::Uuid resource{};
  std::string name{};
  int amount{};

  bool operator==(const PlayerResource &rhs) const = default;
};

} // namespace bsgalone::server
