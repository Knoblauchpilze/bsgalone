
#include "PlayerResourceData.hh"

namespace bsgo {

bool PlayerResourceData::operator==(const PlayerResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
