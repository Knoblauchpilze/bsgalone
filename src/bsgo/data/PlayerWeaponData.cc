
#include "PlayerWeaponData.hh"

namespace bsgo {

bool PlayerWeaponData::operator==(const PlayerWeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
