
#include "PlayerShip.hh"

namespace bsgalone::core {

bool PlayerShip::operator==(const PlayerShip &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::core
