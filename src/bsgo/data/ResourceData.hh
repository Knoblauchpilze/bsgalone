
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgo {

struct ResourceData
{
  Uuid dbId{};
  std::string name{};

  bool operator==(const ResourceData &rhs) const;
};

} // namespace bsgo
