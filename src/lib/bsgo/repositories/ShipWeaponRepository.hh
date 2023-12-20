
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <vector>

namespace bsgo {

struct ShipWeapon
{
  Uuid ship{};
  Uuid weapon{};
  Uuid slot{};
  Eigen::Vector3f slotPosition{};
};

class ShipWeaponRepository : public AbstractRepository
{
  public:
  ShipWeaponRepository(const DbConnectionShPtr &connection);
  ~ShipWeaponRepository() override = default;

  void initialize() override;

  auto findOneByShipAndWeapon(const Uuid &ship, const Uuid &weapon) const
    -> std::optional<ShipWeapon>;
  auto findAllByShip(const Uuid &ship) const -> std::vector<ShipWeapon>;

  void save(const ShipWeapon &weapon);
  void deleteByShipAndSlot(const ShipWeapon &weapon);
};

using ShipWeaponRepositoryShPtr = std::shared_ptr<ShipWeaponRepository>;

} // namespace bsgo
