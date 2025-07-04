
#include "PlayerData.hh"

namespace bsgo {

bool PlayerData::operator==(const PlayerData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
