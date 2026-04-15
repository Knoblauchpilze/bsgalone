
#include "System.hh"

namespace bsgalone::server {

bool System::operator==(const System &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::server
