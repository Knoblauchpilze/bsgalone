
#include "WeaponData.hh"

namespace bsgo {

bool WeaponData::operator==(const WeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
