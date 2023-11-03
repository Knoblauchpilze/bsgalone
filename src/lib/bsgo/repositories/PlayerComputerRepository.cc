
#include "PlayerComputerRepository.hh"

namespace bsgo {

PlayerComputerRepository::PlayerComputerRepository()
  : IRepository("computer")
{
  addModule("player");
}

auto PlayerComputerRepository::findOneById(const Uuid &computer) const -> PlayerComputer
{
  PlayerComputer out;

  switch (computer)
  {
    case Uuid{0}:
      out.name       = "Weapon buff";
      out.level      = 1;
      out.offensive  = false;
      out.powerCost  = 20.0f;
      out.reloadTime = utils::Milliseconds(10000);

      out.duration       = {utils::Milliseconds(3500)};
      out.damageModifier = {1.5f};
      break;
    case Uuid{1}:
      out.name       = "Scan";
      out.level      = 5;
      out.offensive  = true;
      out.powerCost  = 5.0f;
      out.range      = {6.0f};
      out.reloadTime = utils::Milliseconds(2000);

      out.duration       = {};
      out.allowedTargets = {{EntityKind::ASTEROID}};
      out.damageModifier = {};
      break;
    case Uuid{2}:
      out.name       = "Weapon buff";
      out.level      = 6;
      out.offensive  = false;
      out.powerCost  = 18.0f;
      out.reloadTime = utils::Milliseconds(8000);

      out.duration       = {utils::Milliseconds(4000)};
      out.damageModifier = {1.7f};
      break;
    default:
      error("Computer " + str(computer) + " not found");
      break;
  }

  return out;
}

auto PlayerComputerRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {Uuid{0}, Uuid{1}, Uuid{2}};
}

} // namespace bsgo
