
#include "Asteroid.hh"

namespace bsgalone::core {

bool Asteroid::operator==(const Asteroid &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::core
