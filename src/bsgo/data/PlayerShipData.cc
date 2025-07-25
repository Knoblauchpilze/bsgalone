
#include "PlayerShipData.hh"

namespace bsgo {

bool PlayerShipData::operator==(const PlayerShipData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
