
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
    case 0:
      out.level      = 1;
      out.offensive  = false;
      out.powerCost  = 20.0f;
      out.reloadTime = utils::Milliseconds(10000);

      out.duration       = {utils::Milliseconds(3500)};
      out.damageModifier = {1.5f};
      break;
    case 1:
      out.level      = 5;
      out.offensive  = true;
      out.powerCost  = 5.0f;
      out.range      = {6.0f};
      out.reloadTime = utils::Milliseconds(2000);

      out.duration       = {};
      out.allowedTargets = {{EntityKind::ASTEROID}};
      out.damageModifier = {};
      break;
    default:
      error("Computer " + str(computer) + " not found");
      break;
  }

  return out;
}

auto PlayerComputerRepository::findAllByPlayer(const Uuid &player) const -> std::vector<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {};
}

} // namespace bsgo
