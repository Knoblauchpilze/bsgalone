
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

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData;

} // namespace bsgo
