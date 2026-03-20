
#include "Account.hh"

namespace bsgalone::core {

bool Account::operator==(const Account &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::core
