
#include "OutpostData.hh"

namespace bsgo {

bool OutpostData::operator==(const OutpostData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
