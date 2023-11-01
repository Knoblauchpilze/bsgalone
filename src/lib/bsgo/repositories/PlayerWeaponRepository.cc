
#include "PlayerWeaponRepository.hh"

namespace bsgo {

PlayerWeaponRepository::PlayerWeaponRepository()
  : IRepository("weapon")
{
  addModule("player");
}

auto PlayerWeaponRepository::findAllById(const Uuid &player) const -> std::vector<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {};
}

} // namespace bsgo
