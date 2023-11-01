
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace bsgo {

class ShipWeaponRepository : public IRepository
{
  public:
  ShipWeaponRepository();
  ~ShipWeaponRepository() override = default;

  auto findAllById(const Uuid &ship) const -> std::vector<Uuid>;
};

using ShipWeaponRepositoryShPtr = std::shared_ptr<ShipWeaponRepository>;

} // namespace bsgo
