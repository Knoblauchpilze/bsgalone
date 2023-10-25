

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository()
  : IRepository("computer")
{}

auto ComputerRepository::findOneById(const Uuid &computer) const -> Computer
{
  Computer out;

  switch (computer)
  {
    case 0:
      out.offensive  = false;
      out.powerCost  = 20.0f;
      out.reloadTime = utils::Milliseconds(10000);

      out.duration       = {utils::Milliseconds(3500)};
      out.damageModifier = {1.5f};
      break;
    case 1:
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

} // namespace bsgo
