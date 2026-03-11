
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgalone::core {

struct ResourceData
{
  Uuid dbId{};
  std::string name{};

  bool operator==(const ResourceData &rhs) const;
};

auto operator<<(std::ostream &out, const ResourceData &data) -> std::ostream &;
bool operator>>(std::istream &in, ResourceData &data);

} // namespace bsgalone::core
