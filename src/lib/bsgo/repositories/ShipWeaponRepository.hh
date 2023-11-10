
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

class ShipWeaponRepository : public AbstractRepository
{
  public:
  ShipWeaponRepository(const DbConnectionShPtr &connection);
  ~ShipWeaponRepository() override = default;

  auto findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>;
};

using ShipWeaponRepositoryShPtr = std::shared_ptr<ShipWeaponRepository>;

} // namespace bsgo
