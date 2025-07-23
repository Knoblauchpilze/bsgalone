
#include "ResourceData.hh"

namespace bsgo {

bool ResourceData::operator==(const ResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
