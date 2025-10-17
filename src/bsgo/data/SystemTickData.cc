
#include "SystemTickData.hh"

namespace bsgo {

bool SystemTickData::operator==(const SystemTickData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
