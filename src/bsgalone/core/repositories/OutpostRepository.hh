
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

struct Outpost
{
  Faction faction{};

  float hullPoints{0.0f};
  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float powerPoints{0.0f};
  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float radius{2.0f};
};

class OutpostRepository : public AbstractRepository
{
  public:
  OutpostRepository(const DbConnectionShPtr &connection);
  ~OutpostRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid outpost) const -> Outpost;
};

using OutpostRepositoryShPtr = std::shared_ptr<OutpostRepository>;

} // namespace bsgo
