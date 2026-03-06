
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgalone::core {

struct ResourceData
{
  Uuid dbId{};
  std::string name{};

  bool operator==(const ResourceData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgalone::core
