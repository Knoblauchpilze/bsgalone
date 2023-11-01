
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository()
  : IRepository("weapon")
{
  addModule("ship");
}

auto ShipWeaponRepository::findAllById(const Uuid & /*player*/) const -> std::vector<Uuid>
{
  return {};
}

} // namespace bsgo
