
#include "ComputerData.hh"

namespace bsgo {

bool ComputerData::operator==(const ComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbComputer(const Computer &computer, const ComputerPriceRepository &repository)
  -> ComputerData
{
  ComputerData out{
    .dbId           = computer.id,
    .name           = computer.name,
    .offensive      = computer.offensive,
    .powerCost      = computer.powerCost,
    .range          = computer.range,
    .reloadTime     = computer.reloadTime,
    .duration       = computer.duration,
    .allowedTargets = computer.allowedTargets,
    .damageModifier = computer.damageModifier,
  };

  for (const auto &[resource, cost] : repository.findAllByComputer(computer.id))
  {
    out.price[resource] = cost;
  }

  return out;
}

} // namespace bsgo
