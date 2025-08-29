
#pragma once

#include "PlayerResourceRepository.hh"
#include "Uuid.hh"
#include <string>

namespace bsgo {

struct PlayerResourceData
{
  Uuid dbId{};
  std::string name{};
  int amount{};

  bool operator==(const PlayerResourceData &rhs) const;
};

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData;

} // namespace bsgo
