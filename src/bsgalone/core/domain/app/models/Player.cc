
#include "Player.hh"

namespace bsgalone::core {

bool Player::operator==(const Player &rhs) const
{
  return dbId == rhs.dbId;
}

auto Player::systemDbId() const -> Uuid
{
  const auto maybeActiveShip = std::find_if(ships.begin(), ships.end(), [](const PlayerShip &ship) {
    return ship.active;
  });

  if (maybeActiveShip == ships.end())
  {
    throw std::runtime_error("Player " + str(dbId) + " does not have an active ship");
  }
  if (!maybeActiveShip->system.has_value())
  {
    throw std::runtime_error("Player " + str(dbId) + " has an active ship without system");
  }

  return maybeActiveShip->system.value();
}

} // namespace bsgalone::core
