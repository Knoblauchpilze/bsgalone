
#include "Player.hh"

namespace bsgalone::server {

bool Player::operator==(const Player &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::server
