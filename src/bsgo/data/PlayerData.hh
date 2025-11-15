
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgo {

struct PlayerData
{
  Uuid dbId{};
  std::string name{};
  bool isAi{};

  bool operator==(const PlayerData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
