
#include "PlayerWeaponRepository.hh"

namespace bsgo {

PlayerWeaponRepository::PlayerWeaponRepository()
  : IRepository("weapon")
{
  addModule("player");
}

auto PlayerWeaponRepository::findOneById(const Uuid &weapon) const -> PlayerWeapon
{
  PlayerWeapon out;
  out.powerCost = 2.0f;

  out.range = 2.3f;

  constexpr auto RELOAD_TIME_IN_MS = 500;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  switch (weapon)
  {
    case Uuid{0}:
      out.name      = "Medium range cannon";
      out.level     = 2;
      out.minDamage = 1.1f;
      out.maxDamage = 11.0f;
      break;
    case Uuid{1}:
      out.name      = "Short range cannon";
      out.level     = 1;
      out.minDamage = 1.0f;
      out.maxDamage = 10.0f;
      break;
    case Uuid{2}:
      out.name      = "Long range cannon";
      out.level     = 6;
      out.minDamage = 1.6f;
      out.maxDamage = 16.0f;
      break;
    case Uuid{3}:
      out.name      = "Long range (P) cannon";
      out.level     = 7;
      out.minDamage = 1.7f;
      out.maxDamage = 17.0f;
      break;
    case Uuid{4}:
      out.name      = "Medium range cannon";
      out.level     = 20;
      out.minDamage = 2.5f;
      out.maxDamage = 25.0f;
      break;
    default:
      error("Weapon " + str(weapon) + " not found");
      break;
  }

  return out;
}

auto PlayerWeaponRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }

  return {Uuid{0}, Uuid{1}, Uuid{2}, Uuid{3}};
}

} // namespace bsgo
