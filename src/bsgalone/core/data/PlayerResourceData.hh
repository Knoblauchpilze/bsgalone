
#pragma once

#include "PlayerResourceRepository.hh"
#include "Uuid.hh"
#include <string>

namespace bsgalone::core {

struct PlayerResourceData
{
  Uuid dbId{};
  std::string name{};
  int amount{};

  bool operator==(const PlayerResourceData &rhs) const;
};

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData;

auto operator<<(std::ostream &out, const PlayerResourceData &data) -> std::ostream &;
bool operator>>(std::istream &in, PlayerResourceData &data);

} // namespace bsgalone::core
