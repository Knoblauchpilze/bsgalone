
#include "PlayerComputerData.hh"

namespace bsgo {

bool PlayerComputerData::operator==(const PlayerComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
