
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgo {

struct PlayerResourceData
{
  Uuid dbId{};
  std::string name{};
  float amount{};

  bool operator==(const PlayerResourceData &rhs) const;
};

} // namespace bsgo
