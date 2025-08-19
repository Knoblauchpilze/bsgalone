
#include "ShipData.hh"

namespace bsgo {

bool ShipData::operator==(const ShipData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
