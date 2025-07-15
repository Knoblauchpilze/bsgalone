
#include "SystemData.hh"

namespace bsgo {

bool SystemData::operator==(const SystemData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
