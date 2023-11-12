

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{}

auto ComputerRepository::findAll() const -> std::unordered_set<Uuid>
{
  return {Uuid{0}};
}

auto ComputerRepository::findOneById(const Uuid &computer) const -> Computer
{
  Computer out;

  switch (computer)
  {
    case Uuid{0}:
      out.name       = "Weapon debuff";
      out.offensive  = false;
      out.powerCost  = 20.0f;
      out.reloadTime = utils::Milliseconds(10000);

      out.duration       = {utils::Milliseconds(3500)};
      out.damageModifier = {0.5f};
      break;
    case Uuid{1}:
      out.name       = "Scan";
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
