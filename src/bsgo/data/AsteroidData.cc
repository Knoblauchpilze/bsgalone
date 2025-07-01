
#include "AsteroidData.hh"

namespace bsgo {

bool AsteroidData::operator==(const AsteroidData &rhs) const
{
  // Equality only depends on the database identifier.
  return dbId == rhs.dbId;
}

} // namespace bsgo
