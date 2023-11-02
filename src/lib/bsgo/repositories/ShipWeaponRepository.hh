
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class ShipWeaponRepository : public IRepository
{
  public:
  ShipWeaponRepository();
  ~ShipWeaponRepository() override = default;

  auto findAllByShip(const Uuid &ship) const -> std::vector<Uuid>;
};

using ShipWeaponRepositoryShPtr = std::shared_ptr<ShipWeaponRepository>;

} // namespace bsgo
