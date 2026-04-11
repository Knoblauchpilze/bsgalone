
#include "Account.hh"

namespace bsgalone::server {

bool Account::operator==(const Account &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::server
