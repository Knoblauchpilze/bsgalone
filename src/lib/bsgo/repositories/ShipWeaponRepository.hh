
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace bsgo {

struct ShipWeapon
{
  Uuid ship{};
  Uuid weapon{};
  Eigen::Vector3f slotPosition{};
};

class ShipWeaponRepository : public AbstractRepository
{
  public:
  ShipWeaponRepository(const DbConnectionShPtr &connection);
  ~ShipWeaponRepository() override = default;

  void initialize() override;

  auto findAllByShip(const Uuid &ship) const -> std::vector<ShipWeapon>;
};

using ShipWeaponRepositoryShPtr = std::shared_ptr<ShipWeaponRepository>;

} // namespace bsgo
