
#include "ComputerData.hh"

namespace bsgo {

bool ComputerData::operator==(const ComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgo
