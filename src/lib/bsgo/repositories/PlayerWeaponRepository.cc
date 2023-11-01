
#include "PlayerWeaponRepository.hh"

namespace bsgo {

PlayerWeaponRepository::PlayerWeaponRepository()
  : IRepository("weapon")
{
  addModule("player");
}

auto PlayerWeaponRepository::findAllById(const Uuid & /*player*/) const -> std::vector<Uuid>
{
  return {};
}

} // namespace bsgo
