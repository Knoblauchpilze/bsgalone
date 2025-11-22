
#pragma once

#include "EntityKind.hh"
#include "Uuid.hh"

namespace bsgo {

struct TargetData
{
  Uuid sourceDbId{};
  EntityKind sourceKind{};
  Uuid targetDbId{};
  EntityKind targetKind{};

  bool operator==(const TargetData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
