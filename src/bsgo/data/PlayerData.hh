
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgo {

struct PlayerData
{
  Uuid dbId{};
  std::string name{};

  bool operator==(const PlayerData &rhs) const;
};

} // namespace bsgo
