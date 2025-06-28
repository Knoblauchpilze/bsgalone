
#pragma once

#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct OutpostData
{
  Uuid dbId{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float hullPoints{};
  float powerPoints{};
  Faction faction{};

  std::optional<Uuid> targetDbId{};

  bool operator==(const OutpostData &rhs) const;
};

auto serializeOutpostData(std::ostream &out, const OutpostData &data) -> std::ostream &;
bool deserializeOutpostData(std::istream &in, OutpostData &data);

} // namespace bsgo
