
#include "PlayerCredentials.hh"

namespace bsgalone::client {

bool PlayerCredentials::valid() const
{
  return !username.empty() && !password.empty();
}

} // namespace bsgalone::client
