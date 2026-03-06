
#include "Components.hh"

namespace bsgalone::core {

void Components::clear()
{
  transforms.clear();
  velocities.clear();
  healths.clear();
  powers.clear();
  targets.clear();
  kinds.clear();
  factions.clear();
  loots.clear();
  scanned.clear();
  owners.clear();
  damages.clear();
  removals.clear();
  statuses.clear();
  ais.clear();
  shipClasses.clear();
  names.clear();
  networkSyncs.clear();
  dbs.clear();
  dbSyncs.clear();
  weapons.clear();
  computers.clear();
  effects.clear();
  resources.clear();
}

} // namespace bsgalone::core
