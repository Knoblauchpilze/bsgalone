
#include "System.hh"

namespace bsgalone::core {

bool System::operator==(const System &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::core
