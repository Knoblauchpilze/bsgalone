
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

struct Ship
{
  float hullPoints{0.0f};
  float powerPoints{0.0f};
  float acceleration{5.0f};
  float radius{0.5f};
};

class ShipRepository : public IRepository
{
  public:
  ShipRepository();
  ~ShipRepository() override = default;

  auto findOneById(const Uuid &id) const -> Ship;
  auto findPositionById(const Uuid &id) const -> Eigen::Vector3f;
};

using ShipRepositoryShPtr = std::shared_ptr<ShipRepository>;

} // namespace bsgo
