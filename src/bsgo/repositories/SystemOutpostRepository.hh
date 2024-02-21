
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

struct SystemOutpost
{
  Faction faction{};
  Uuid system{};

  float hullPoints{0.0f};
  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float powerPoints{0.0f};
  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float radius{2.0f};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
};

class SystemOutpostRepository : public AbstractRepository
{
  public:
  SystemOutpostRepository(const DbConnectionShPtr &connection);
  ~SystemOutpostRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid outpost) const -> SystemOutpost;
};

using SystemOutpostRepositoryShPtr = std::shared_ptr<SystemOutpostRepository>;

} // namespace bsgo
