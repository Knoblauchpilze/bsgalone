
#include "Player.hh"

namespace bsgalone::core {

bool Player::operator==(const Player &rhs) const
{
  return dbId == rhs.dbId;
}

} // namespace bsgalone::core
